#include "ASConsole.h"
#include <sys/stat.h>

namespace AStyle
{
/**
 * Process files in the fileNameVector.
 */
void ASConsole::ProcessFiles()
{
	clock_t startTime=0;
	if(GetIsVerbose())
		startTime = PrintVerboseHeader();
	// loop thru input fileNameVector and process the files
	for(size_t i = 0; i < m_listInputFilePaths.size(); i++)
	{
		ProcessInputFilePaths(m_listInputFilePaths[i]);
		// loop thru fileName vector formatting the files
		for(size_t j = 0; j < fileName.size(); j++)
			formatFile(fileName[j]);
	}
	// files are processed, display stats
	if(GetIsVerbose())
		PrintVerboseStats(startTime);
}

clock_t ASConsole::PrintVerboseHeader() const
{
	clock_t startTime = clock();     // start time of file formatting
	assert(m_bIsVerbose);
	if(isQuiet)
		return startTime;
	// get the date
	struct tm* ptr;
	time_t lt;
	char str[20];
	lt = time(NULL);
	ptr = localtime(&lt);
	strftime(str, 20, "%x", ptr);
	// print the header
	printf("Artistic Style %s     %s\n", g_strVersion, str);
	// print options file
	if(!OptionsFileNameIsEmpty())
		printf(_("Using default options file %s\n"), GetOptionsFileName());
	return startTime;
}
void ASConsole::PrintVerboseStats(clock_t startTime) const
{
	assert(m_bIsVerbose);
	if(isQuiet)
		return;
	if(hasWildcard)
		PrintSeparatingLine();
	string formatted = GetNumFormat(filesFormatted);
	string unchanged = GetNumFormat(filesUnchanged);
	printf(_(" %s formatted   %s unchanged   "), formatted.c_str(), unchanged.c_str());
	// show processing time
	clock_t stopTime = clock();
	float secs = (stopTime - startTime) / float (CLOCKS_PER_SEC);
	if(secs < 60)
	{
		if(secs < 2.0)
			printf("%.2f", secs);
		else if(secs < 20.0)
			printf("%.1f", secs);
		else
			printf("%.0f", secs);
		printf("%s", _(" seconds   "));
	}
	else
	{
		// show minutes and seconds if time is greater than one minute
		int min = (int) secs / 60;
		secs -= min * 60;
		int minsec = int (secs + .5);
		printf(_("%d min %d sec   "), min, minsec);
	}
	string lines = GetNumFormat(linesOut);
	printf(_("%s lines\n"), lines.c_str());
}
void ASConsole::PrintSeparatingLine(int nNum) const
{
	string line;
	for(int i = 0; i < nNum; i++)
		line.append("-");
	printMsg("%s\n", line);
}

size_t ASConsole::SplitFilePath(string &strFilePath,string &strFileDirectory,string &strFileName)
{
	size_t separator = strFilePath.find_last_of(g_fileSeparator);
	if(separator == string::npos)
	{
		// if no directory is present, use the currently active directory
		strFileDirectory = GetCurDirectory(strFilePath);
		strFileName  = strFilePath;
	}
	else
	{
		strFileDirectory = strFilePath.substr(0, separator);
		strFileName  = strFilePath.substr(separator + 1);
	}
	return strFileDirectory.length() + 1;    // +1 includes trailing separator
}

// get individual file names from the command-line file path
void ASConsole::ProcessInputFilePaths(string &filePath)
{
	fileName.clear();
	mainDirectoryLength = SplitFilePath(filePath,targetDirectory,targetFilename);
	if(targetFilename.length() == 0)
	{
		fprintf(stderr, _("Missing filename in %s\n"), filePath.c_str());
		error();
	}
	// check filename for wildcards
	hasWildcard = false;
	if(targetFilename.find_first_of("*?") != string::npos)
		hasWildcard = true;
	// clear exclude hits vector
	for(size_t ix = 0; ix < excludeHitsVector.size(); ix++)
		excludeHitsVector[ix] = false;
	// If the filename is not quoted on Linux, bash will replace the
	// wildcard instead of passing it to the program.
	if(isRecursive && !hasWildcard)
	{
		fprintf(stderr, "%s\n", _("Recursive option with no wildcard"));
#ifndef _WIN32
		fprintf(stderr, "%s\n", _("Did you intend quote the filename"));
#endif
		error();
	}
	// display directory name for wildcard processing
	if(hasWildcard)
	{
		PrintSeparatingLine();
		printMsg(_("Directory  %s\n"), targetDirectory + g_fileSeparator + targetFilename);
	}
	// create a vector of paths and file names to process
	if(hasWildcard || isRecursive)
		GetFileListInDirectory(targetDirectory, targetFilename);
	else
	{
		// verify a single file is not a directory (needed on Linux)
		string strFilePath = targetDirectory + g_fileSeparator + targetFilename;
		if(IsExcluedFileName(targetFilename))
			printMsg(_("Exclude File  %s\n"), strFilePath.substr(mainDirectoryLength));
		else
		{
			struct stat statbuf;
			if(stat(strFilePath.c_str(), &statbuf) == 0 && (statbuf.st_mode & S_IFREG))
				fileName.push_back(strFilePath);
		}
	}
	// check for unprocessed excludes
	bool excludeErr = false;
	for(size_t ix = 0; ix < excludeHitsVector.size(); ix++)
	{
		if(excludeHitsVector[ix] == false)
		{
			excludeErr = true;
			if(!m_bIgnoreExcludeErrors)
				fprintf(stderr, _("Exclude (unmatched)  %s\n"), m_listExclude[ix].c_str());
			else if(!m_bIgnoreExcludeErrorsDisplay)
				printMsg(_("Exclude (unmatched)  %s\n"), m_listExclude[ix]);
		}
	}
	if(excludeErr && !m_bIgnoreExcludeErrors)
	{
		if(hasWildcard && !isRecursive)
			fprintf(stderr, "%s\n", _("Did you intend to use --recursive"));
		error();
	}
	if(hasWildcard)
		PrintSeparatingLine();
	// check if files were found (probably an input error if not)
	if(fileName.empty())
	{
		if(!m_bIgnoreEmptyErrors)
		{
			fprintf(stderr, _("No file to process %s\n"), filePath.c_str());
			if(hasWildcard && !isRecursive)
				fprintf(stderr, "%s\n", _("Did you intend to use --recursive"));
			error();
		}
		else if(!m_bIgnoreEmptyErrorsDisplay)
		{
			printMsg(_("No file to process %s\n"), filePath.c_str());
			if(hasWildcard && !isRecursive)
				printMsg("%s\n", _("Did you intend to use --recursive"));
		}
	}
}

// compare a path to the exclude vector
// used for both directories and filenames
// updates the g_excludeHitsVector
// return true if a match
bool ASConsole::IsExcluedDirectory(const string &strDirectoryPath)
{
	bool retVal = false;
	// read the exclude vector checking for a match
	for(size_t i = 0; i < m_listExclude.size(); i++)
	{
		if(m_listExcludeFnD[i])
			continue;
		string strExclude = m_listExclude[i];
		if(strDirectoryPath.length() < strExclude.length())
			continue;
		size_t nCompareStart = strDirectoryPath.length() - strExclude.length();
		// subPath compare must start with a directory name
		if(nCompareStart > 0)
		{
			char lastPathChar = strDirectoryPath[nCompareStart - 1];
			if(lastPathChar != g_fileSeparator)
				continue;
		}
		string strCompare = strDirectoryPath.substr(nCompareStart);
		if(!g_isCaseSensitive)
		{
			// make it case insensitive for Windows
			for(size_t j = 0; j < strCompare.length(); j++)
				strCompare[j] = (char)tolower(strCompare[j]);
			for(size_t j = 0; j < strExclude.length(); j++)
				strExclude[j] = (char)tolower(strExclude[j]);
		}
		// compare sub directory to exclude data - must check them all
		if(strCompare == strExclude)
		{
			excludeHitsVector[i] = true;
			retVal = true;
			break;
		}
	}
	return retVal;
}

bool ASConsole::IsExcluedFileName(const string &strFileName)
{
	bool retVal = false;
	// read the exclude vector checking for a match
	for(size_t i = 0; i < m_listExclude.size(); i++)
	{
		if(!m_listExcludeFnD[i])
			continue;
		string strWildcard = m_listExclude[i];
		if(WildCmp(strWildcard.c_str(), strFileName.c_str()))
		{
			excludeHitsVector[i] = true;
			retVal = true;
			break;
		}
	}
	return retVal;
}

}   // end of astyle namespace
