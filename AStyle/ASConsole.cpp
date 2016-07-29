#include <fstream>

#include "ASConsole.h"
#include "ASStreamIterator.inl"

namespace AStyle
{
//-----------------------------------------------------------------------------
// ASConsole class
// main function will be included only in the console build
//-----------------------------------------------------------------------------
ASConsole::ASConsole(void) : m_lpErrorOut(&cerr)
{
	//命令行参数初始化
	m_bIsVerbose = false;
	isRecursive = false;
	isDryRun = false;
	noBackup = false;
	preserveDate = false;
	isQuiet = false;
	isFormattedOnly = false;
	m_bIgnoreExcludeErrors = false;
	m_bIgnoreExcludeErrorsDisplay = false;
	m_bIgnoreEmptyErrors = false;
	m_bIgnoreEmptyErrorsDisplay = false;
	useAscii = false;
	// other variables
	bypassBrowserOpen = false;
	hasWildcard = false;
	filesAreIdentical = true;
	lineEndsMixed = false;
	mainDirectoryLength = 0;
	filesFormatted = 0;
	filesUnchanged = 0;
	linesOut = 0;
	outputEOL[0] = '\0';
	prevEOL[0] = '\0';
	origSuffix = ".orig";
}

int ASConsole::DoMain(int argc, _TCHAR* argv[])
{
	// process command line and options file
	// build the vectors fileNameVector, optionsVector, and fileOptionsVector
	vector<string> listOptions;
	GetArgvOptions(argc, argv,listOptions);
	ProcessOptions(listOptions);
	if(m_listInputFilePaths.empty())            // if no files have been given, use cin for input and cout for output
		formatCinToCout();
	else                                    // else process entries in the fileNameVector
		ProcessFiles();
	return EXIT_SUCCESS;
}

// error exit without a message
void ASConsole::error() const
{
	(*m_lpErrorOut) << _("\nArtistic Style has terminated") << endl;
	exit(EXIT_FAILURE);
}

// error exit with a message
void ASConsole::error(const char* why, const char* what) const
{
	(*m_lpErrorOut) << why << ' ' << what << endl;
	error();
}

// rewrite a stringstream converting the line ends
void ASConsole::convertLineEnds(ostringstream &out, int lineEnd)
{
	assert(lineEnd == LINEEND_WINDOWS || lineEnd == LINEEND_LINUX || lineEnd == LINEEND_MACOLD);
	const string &inStr = out.str();    // avoids strange looking syntax
	string outStr;                      // the converted output
	int inLength = inStr.length();
	for(int pos = 0; pos < inLength; pos++)
	{
		if(inStr[pos] == '\r')
		{
			if(inStr[pos + 1] == '\n')
			{
				// CRLF
				if(lineEnd == LINEEND_CR)
				{
					outStr += inStr[pos];       // Delete the LF
					pos++;
					continue;
				}
				else if(lineEnd == LINEEND_LF)
				{
					outStr += inStr[pos + 1];       // Delete the CR
					pos++;
					continue;
				}
				else
				{
					outStr += inStr[pos];       // Do not change
					outStr += inStr[pos + 1];
					pos++;
					continue;
				}
			}
			else
			{
				// CR
				if(lineEnd == LINEEND_CRLF)
				{
					outStr += inStr[pos];       // Insert the CR
					outStr += '\n';             // Insert the LF
					continue;
				}
				else if(lineEnd == LINEEND_LF)
				{
					outStr += '\n';             // Insert the LF
					continue;
				}
				else
				{
					outStr += inStr[pos];       // Do not change
					continue;
				}
			}
		}
		else if(inStr[pos] == '\n')
		{
			// LF
			if(lineEnd == LINEEND_CRLF)
			{
				outStr += '\r';             // Insert the CR
				outStr += inStr[pos];       // Insert the LF
				continue;
			}
			else if(lineEnd == LINEEND_CR)
			{
				outStr += '\r';             // Insert the CR
				continue;
			}
			else
			{
				outStr += inStr[pos];       // Do not change
				continue;
			}
		}
		else
		{
			outStr += inStr[pos];       // Write the current char
		}
	}
	// replace the stream
	out.str(outStr);
}

void ASConsole::correctMixedLineEnds(ostringstream &out)
{
	LineEndFormat lineEndFormat = LINEEND_DEFAULT;
	if(strcmp(outputEOL, "\r\n") == 0)
		lineEndFormat = LINEEND_WINDOWS;
	if(strcmp(outputEOL, "\n") == 0)
		lineEndFormat = LINEEND_LINUX;
	if(strcmp(outputEOL, "\r") == 0)
		lineEndFormat = LINEEND_MACOLD;
	convertLineEnds(out, lineEndFormat);
}

// check files for 16 or 32 bit encoding
// the file must have a Byte Order Mark (BOM)
// NOTE: some string functions don't work with NULLs (e.g. length())
FileEncoding ASConsole::detectEncoding(const char* data, size_t dataSize) const
{
	FileEncoding encoding = ENCODING_8BIT;
	if(dataSize >= 4 && memcmp(data, "\x00\x00\xFE\xFF", 4) == 0)
		encoding = UTF_32BE;
	else if(dataSize >= 4 && memcmp(data, "\xFF\xFE\x00\x00", 4) == 0)
		encoding = UTF_32LE;
	else if(dataSize >= 2 && memcmp(data, "\xFE\xFF", 2) == 0)
		encoding = UTF_16BE;
	else if(dataSize >= 2 && memcmp(data, "\xFF\xFE", 2) == 0)
		encoding = UTF_16LE;
	return encoding;
}

/**
 * If no files have been given, use cin for input and cout for output.
 *
 * This is used to format text for text editors like TextWrangler (Mac).
 * Do NOT display any console messages when this function is used.
 */
void ASConsole::formatCinToCout()
{
	// Using cin.tellg() causes problems with both Windows and Linux.
	// The Windows problem occurs when the input is not Windows line-ends.
	// The tellg() will be out of sequence with the get() statements.
	// The Linux cin.tellg() will return -1 (invalid).
	// Copying the input sequentially to a stringstream before
	// formatting solves the problem for both.
	istream* inStream = &cin;
	stringstream outStream;
	char ch;
	inStream->get(ch);
	while(!inStream->eof())
	{
		outStream.put(ch);
		inStream->get(ch);
	}
	ASStreamIterator<stringstream> streamIterator(&outStream);
	// Windows pipe or redirection always outputs Windows line-ends.
	// Linux pipe or redirection will output any line end.
	LineEndFormat lineEndFormat = m_Formatter.getLineEndFormat();
	initializeOutputEOL(lineEndFormat);
	m_Formatter.init(&streamIterator);
	while(m_Formatter.hasMoreLines())
	{
		cout << m_Formatter.nextLine();
		if(m_Formatter.hasMoreLines())
		{
			setOutputEOL(lineEndFormat, streamIterator.getOutputEOL());
			cout << outputEOL;
		}
		else
		{
			// this can happen if the file if missing a closing bracket and break-blocks is requested
			if(m_Formatter.getIsLineReady())
			{
				setOutputEOL(lineEndFormat, streamIterator.getOutputEOL());
				cout << outputEOL;
				cout << m_Formatter.nextLine();
			}
		}
	}
	cout.flush();
}

/**
 * Open input file, format it, and close the output.
 *
 * @param fileName_     The path and name of the file to be processed.
 */
void ASConsole::formatFile(const string &fileName_)
{
	stringstream in;
	ostringstream out;
	FileEncoding encoding = readFile(fileName_, in);
	// Unless a specific language mode has been set, set the language mode
	// according to the file's suffix.
	if(!m_Formatter.getModeManuallySet())
	{
		if(stringEndsWith(fileName_, string(".java")))
			m_Formatter.setJavaStyle();
		else if(stringEndsWith(fileName_, string(".cs")))
			m_Formatter.setSharpStyle();
		else
			m_Formatter.setCStyle();
	}
	// set line end format
	string nextLine;                // next output line
	filesAreIdentical = true;       // input and output files are identical
	LineEndFormat lineEndFormat = m_Formatter.getLineEndFormat();
	initializeOutputEOL(lineEndFormat);
	// do this AFTER setting the file mode
	ASStreamIterator<stringstream> streamIterator(&in);
	m_Formatter.init(&streamIterator);
	// format the file
	while(m_Formatter.hasMoreLines())
	{
		nextLine = m_Formatter.nextLine();
		out << nextLine;
		linesOut++;
		if(m_Formatter.hasMoreLines())
		{
			setOutputEOL(lineEndFormat, streamIterator.getOutputEOL());
			out << outputEOL;
		}
		else
		{
			streamIterator.saveLastInputLine();     // to compare the last input line
			// this can happen if the file if missing a closing bracket and break-blocks is requested
			if(m_Formatter.getIsLineReady())
			{
				setOutputEOL(lineEndFormat, streamIterator.getOutputEOL());
				out << outputEOL;
				nextLine = m_Formatter.nextLine();
				out << nextLine;
				linesOut++;
				streamIterator.saveLastInputLine();
			}
		}
		if(filesAreIdentical)
		{
			if(streamIterator.checkForEmptyLine)
			{
				if(nextLine.find_first_not_of(" \t") != string::npos)
					filesAreIdentical = false;
			}
			else if(!streamIterator.compareToInputBuffer(nextLine))
				filesAreIdentical = false;
			streamIterator.checkForEmptyLine = false;
		}
	}
	// correct for mixed line ends
	if(lineEndsMixed)
	{
		correctMixedLineEnds(out);
		filesAreIdentical = false;
	}
	// remove targetDirectory from filename if required by print
	string displayName;
	if(hasWildcard)
		displayName = fileName_.substr(targetDirectory.length() + 1);
	else
		displayName = fileName_;
	// if file has changed, write the new file
	if(!filesAreIdentical || streamIterator.getLineEndChange(lineEndFormat))
	{
		if(!isDryRun)
			writeFile(fileName_, encoding, out);
		printMsg(_("Formatted  %s\n"), displayName);
		filesFormatted++;
	}
	else
	{
		if(!isFormattedOnly)
			printMsg(_("Unchanged  %s\n"), displayName);
		filesUnchanged++;
	}
	assert(m_Formatter.getChecksumDiff() == 0);
}

// for unit testing
bool ASConsole::getFilesAreIdentical() const
{
	return filesAreIdentical;
}

// for unit testing
int ASConsole::getFilesFormatted() const
{
	return filesFormatted;
}

// for unit testing
bool ASConsole::getIsDryRun() const
{
	return isDryRun;
}

// for unit testing
bool ASConsole::getIsFormattedOnly() const
{
	return isFormattedOnly;
}


// for unit testing
bool ASConsole::getIsQuiet() const
{
	return isQuiet;
}

// for unit testing
bool ASConsole::getIsRecursive() const
{
	return isRecursive;
}

// for unit testing
bool ASConsole::getLineEndsMixed() const
{
	return lineEndsMixed;
}

// for unit testing
bool ASConsole::getNoBackup() const
{
	return noBackup;
}

// for unit testing
string ASConsole::getOrigSuffix() const
{
	return origSuffix;
}

// for unit testing
bool ASConsole::getPreserveDate() const
{
	return preserveDate;
}

// for unit testing
void ASConsole::setBypassBrowserOpen(bool state)
{
	bypassBrowserOpen = state;
}

// initialize output end of line
void ASConsole::initializeOutputEOL(LineEndFormat lineEndFormat)
{
	assert(lineEndFormat == LINEEND_DEFAULT
		   || lineEndFormat == LINEEND_WINDOWS
		   || lineEndFormat == LINEEND_LINUX
		   || lineEndFormat == LINEEND_MACOLD);
	outputEOL[0] = '\0';        // current line end
	prevEOL[0] = '\0';          // previous line end
	lineEndsMixed = false;      // output has mixed line ends, LINEEND_DEFAULT only
	if(lineEndFormat == LINEEND_WINDOWS)
		strcpy(outputEOL, "\r\n");
	else if(lineEndFormat == LINEEND_LINUX)
		strcpy(outputEOL, "\n");
	else if(lineEndFormat == LINEEND_MACOLD)
		strcpy(outputEOL, "\r");
	else
		outputEOL[0] = '\0';
}

FileEncoding ASConsole::readFile(const string &fileName_, stringstream &in) const
{
	const int blockSize = 65536;    // 64 KB
	ifstream fin(fileName_.c_str(), ios::binary);
	if(!fin)
		error("Cannot open input file", fileName_.c_str());
	char* data = new(nothrow) char[blockSize];
	if(!data)
		error("Cannot allocate memory for input file", fileName_.c_str());
	fin.read(data, blockSize);
	if(fin.bad())
		error("Cannot read input file", fileName_.c_str());
	size_t dataSize = static_cast<size_t>(fin.gcount());
	FileEncoding encoding = detectEncoding(data, dataSize);
	if(encoding ==  UTF_32BE || encoding ==  UTF_32LE)
		error(_("Cannot process UTF-32 encoding"), fileName_.c_str());
	bool firstBlock = true;
	bool isBigEndian = (encoding == UTF_16BE);
	while(dataSize)
	{
		if(encoding == UTF_16LE || encoding == UTF_16BE)
		{
			// convert utf-16 to utf-8
			size_t utf8Size = utf8_16.Utf8LengthFromUtf16(data, dataSize, isBigEndian);
			char* utf8Out = new(nothrow) char[utf8Size];
			if(!utf8Out)
				error("Cannot allocate memory for utf-8 conversion", fileName_.c_str());
			size_t utf8Len = utf8_16.Utf16ToUtf8(data, dataSize, isBigEndian, firstBlock, utf8Out);
			assert(utf8Len == utf8Size);
			in << string(utf8Out, utf8Len);
			delete [] utf8Out;
		}
		else
			in << string(data, dataSize);
		fin.read(data, blockSize);
		if(fin.bad())
			error("Cannot read input file", fileName_.c_str());
		dataSize = static_cast<size_t>(fin.gcount());
		firstBlock = false;
	}
	fin.close();
	delete [] data;
	return encoding;
}

void ASConsole::setIsFormattedOnly(bool state)
{
	isFormattedOnly = state;
}

void ASConsole::setIsQuiet(bool state)
{
	isQuiet = state;
}

void ASConsole::setIsRecursive(bool state)
{
	isRecursive = state;
}

void ASConsole::setIsDryRun(bool state)
{
	isDryRun = state;
}

void ASConsole::setNoBackup(bool state)
{
	noBackup = state;
}

void ASConsole::setOrigSuffix(string suffix)
{
	origSuffix = suffix;
}

void ASConsole::setPreserveDate(bool state)
{
	preserveDate = state;
}

// set outputEOL variable
void ASConsole::setOutputEOL(LineEndFormat lineEndFormat, const char* currentEOL)
{
	if(lineEndFormat == LINEEND_DEFAULT)
	{
		strcpy(outputEOL, currentEOL);
		if(strlen(prevEOL) == 0)
			strcpy(prevEOL, outputEOL);
		if(strcmp(prevEOL, outputEOL) != 0)
		{
			lineEndsMixed = true;
			filesAreIdentical = false;
			strcpy(prevEOL, outputEOL);
		}
	}
	else
	{
		strcpy(prevEOL, currentEOL);
		if(strcmp(prevEOL, outputEOL) != 0)
			filesAreIdentical = false;
	}
}

// remove a file and check for an error
void ASConsole::removeFile(const char* fileName_, const char* errMsg) const
{
	if(remove(fileName_))
	{
		if(errno == ENOENT)         // no file is OK
			errno = 0;
		if(errno)
		{
			perror("errno message");
			error(errMsg, fileName_);
		}
	}
}


// make sure file separators are correct type (Windows or Linux)
// remove ending file separator
// remove beginning file separator if requested and NOT a complete file path
void ASConsole::standardizePath(string &path, bool removeBeginningSeparator /*false*/) const
{
#ifdef __VMS
	struct FAB fab;
	struct NAML naml;
	char less[NAML$C_MAXRSS];
	char sess[NAM$C_MAXRSS];
	int r0_status;
	// If we are on a VMS system, translate VMS style filenames to unix
	// style.
	fab = cc$rms_fab;
	fab.fab$l_fna = (char*) - 1;
	fab.fab$b_fns = 0;
	fab.fab$l_naml = &naml;
	naml = cc$rms_naml;
	strcpy(sess, path.c_str());
	naml.naml$l_long_filename = (char*)sess;
	naml.naml$l_long_filename_size = path.length();
	naml.naml$l_long_expand = less;
	naml.naml$l_long_expand_alloc = sizeof(less);
	naml.naml$l_esa = sess;
	naml.naml$b_ess = sizeof(sess);
	naml.naml$v_no_short_upcase = 1;
	r0_status = sys$parse(&fab);
	if(r0_status == RMS$_SYN)
		error("File syntax error", path.c_str());
	else
	{
		if(!$VMS_STATUS_SUCCESS(r0_status))
			(void)lib$signal(r0_status);
	}
	less[naml.naml$l_long_expand_size - naml.naml$b_ver] = '\0';
	sess[naml.naml$b_esl - naml.naml$b_ver] = '\0';
	if(naml.naml$l_long_expand_size > naml.naml$b_esl)
		path = decc$translate_vms(less);
	else
		path = decc$translate_vms(sess);
#endif /* __VMS */
	// make sure separators are correct type (Windows or Linux)
	for(size_t i = 0; i < path.length(); i++)
	{
		i = path.find_first_of("/\\", i);
		if(i == string::npos)
			break;
		path[i] = g_fileSeparator;
	}
	// remove beginning separator if requested
	if(removeBeginningSeparator && (path[0] == g_fileSeparator))
		path.erase(0, 1);
}

void ASConsole::printMsg(const char* msg, const string &data) const
{
	if(isQuiet)
		return;
	printf(msg, data.c_str());
}


bool ASConsole::stringEndsWith(const string &str, const string &suffix) const
{
	int strIndex = (int) str.length() - 1;
	int suffixIndex = (int) suffix.length() - 1;
	while(strIndex >= 0 && suffixIndex >= 0)
	{
		if(tolower(str[strIndex]) != tolower(suffix[suffixIndex]))
			return false;
		--strIndex;
		--suffixIndex;
	}
	// suffix longer than string
	if(strIndex < 0 && suffixIndex >= 0)
		return false;
	return true;
}

void ASConsole::SetExcludeVector(string suffixParam, bool bFnD)
{
	m_listExclude.push_back(suffixParam);
	m_listExcludeFnD.push_back(bFnD);
	standardizePath(m_listExclude.back(), true);
	excludeHitsVector.push_back(false);
}


}   // end of astyle namespace
