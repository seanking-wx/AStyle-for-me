#include "ASConsole.h"
#ifndef _WIN32  // Windows specific
	#error this file use only in windows
#endif
#include <Windows.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include <fstream>


// AStyle平台相关实现函数
namespace AStyle
{

char ASConsole::g_fileSeparator = '\\';     // Windows file separator
bool ASConsole::g_isCaseSensitive = false;  // Windows IS case sensitive

// 调用默认浏览器打开网页
void ASConsole::LaunchDefaultBrowser(const char* filePathIn /*NULL*/) const
{
	struct stat statbuf;
	string htmlDefaultPath;
	const char* envDocPath = getenv("ASTYLE_PATH");
	if(envDocPath != NULL)
	{
		htmlDefaultPath = envDocPath;
		if(htmlDefaultPath.length() > 0
		   && htmlDefaultPath[htmlDefaultPath.length() - 1] == g_fileSeparator)
			htmlDefaultPath.erase(htmlDefaultPath.length() - 1);
		htmlDefaultPath.append("\\doc");
	}
	else
	{
		const char* envPaths[] =
		{
			"PROGRAMFILES(X86)",
			"PROGRAMFILES"
		};
		size_t pathsLen = sizeof(envPaths) / sizeof(envPaths[0]);
		for(size_t i = 0; i < pathsLen; i++)
		{
			const char* envPath = getenv(envPaths[i]);
			if(envPath == NULL)
				continue;
			htmlDefaultPath = envPath;
			if(htmlDefaultPath.length() > 0
			   && htmlDefaultPath[htmlDefaultPath.length() - 1] == g_fileSeparator)
				htmlDefaultPath.erase(htmlDefaultPath.length() - 1);
			htmlDefaultPath.append("\\AStyle\\doc");
			if(stat(htmlDefaultPath.c_str(), &statbuf) == 0 && statbuf.st_mode & S_IFDIR)
				break;
			htmlDefaultPath.clear();
		}
	}
	if(htmlDefaultPath.empty())
		htmlDefaultPath = GetCurDirectory("\\AStyle\\doc");
	if(htmlDefaultPath.length() > 0
	   && htmlDefaultPath[htmlDefaultPath.length()-1] != g_fileSeparator)
		htmlDefaultPath.append("\\");
	// build file path
	string htmlFilePath;
	if(filePathIn == NULL)
		htmlFilePath = htmlDefaultPath + "astyle.html";
	else
	{
		if(strpbrk(filePathIn, "\\/") == NULL)
			htmlFilePath = htmlDefaultPath + filePathIn;
		else
			htmlFilePath = filePathIn;
	}
	standardizePath(htmlFilePath);
	if(stat(htmlFilePath.c_str(), &statbuf) != 0 || !(statbuf.st_mode &S_IFREG))
	{
		error(_("Cannot open HTML file"), htmlFilePath.c_str());
		return;
	}
	SHELLEXECUTEINFOA sei = { sizeof(sei), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	sei.fMask = SEE_MASK_FLAG_NO_UI;
	sei.lpVerb = "open";
	sei.lpFile = htmlFilePath.c_str();
	sei.nShow = SW_SHOWNORMAL;
	// browser open will be bypassed in test programs
	printf(_("Opening HTML documentation %s\n"), htmlFilePath.c_str());
	if(!bypassBrowserOpen)
	{
		int ret = ShellExecuteExA(&sei);
		if(!ret)
			error(_("Command execute failure"), htmlFilePath.c_str());
	}
	exit(EXIT_SUCCESS);
}

void ASConsole::writeFile(const string &fileName_, FileEncoding encoding, ostringstream &out) const
{
	// save date accessed and date modified of original file
	struct stat stBuf;
	bool statErr = false;
	if(stat(fileName_.c_str(), &stBuf) == -1)
		statErr = true;
	// create a backup
	if(!noBackup)
	{
		string origFileName = fileName_ + origSuffix;
		removeFile(origFileName.c_str(), "Cannot remove pre-existing backup file");
		renameFile(fileName_.c_str(), origFileName.c_str(), "Cannot create backup file");
	}
	// write the output file
	ofstream fout(fileName_.c_str(), ios::binary | ios::trunc);
	if(!fout)
		error("Cannot open output file", fileName_.c_str());
	if(encoding == UTF_16LE || encoding == UTF_16BE)
	{
		// convert utf-8 to utf-16
		bool isBigEndian = (encoding == UTF_16BE);
		size_t utf16Size = utf8_16.Utf16LengthFromUtf8(out.str().c_str(), out.str().length());
		char* utf16Out = new char[utf16Size];
		size_t utf16Len = utf8_16.Utf8ToUtf16(const_cast<char*>(out.str().c_str()),
											  out.str().length(), isBigEndian, utf16Out);
		assert(utf16Len == utf16Size);
		fout << string(utf16Out, utf16Len);
		delete [] utf16Out;
	}
	else
		fout << out.str();
	fout.close();
	// change date modified to original file date
	// Embarcadero must be linked with cw32mt not cw32
	if(preserveDate)
	{
		if(!statErr)
		{
			struct utimbuf outBuf;
			outBuf.actime = stBuf.st_atime;
			// add ticks so 'make' will recognize a change
			// Visual Studio 2008 needs more than 1
			outBuf.modtime = stBuf.st_mtime + 10;
			if(utime(fileName_.c_str(), &outBuf) == -1)
				statErr = true;
		}
		if(statErr)
		{
			perror("errno message");
			(*m_lpErrorOut) << "*********  Cannot preserve file date" << endl;
		}
	}
}
// rename a file and check for an error
void ASConsole::renameFile(const char* oldFileName, const char* newFileName, const char* errMsg) const
{
	int result = rename(oldFileName, newFileName);
	if(result != 0)
	{
		// if file still exists the remove needs more time - retry
		if(errno == EEXIST)
		{
			errno = 0;
			waitForRemove(newFileName);
			result = rename(oldFileName, newFileName);
		}
		if(result != 0)
		{
			perror("errno message");
			error(errMsg, oldFileName);
		}
	}
}

int ASConsole::waitForRemove(const char* newFileName) const
{
	struct stat stBuf;
	int seconds;
	// sleep a max of 20 seconds for the remove
	for(seconds = 1; seconds <= 20; seconds++)
	{
		sleep(1);
		if(stat(newFileName, &stBuf) != 0)
			break;
	}
	errno = 0;
	return seconds;
}

void ASConsole::sleep(int seconds) const
{
	clock_t endwait;
	endwait = clock_t (clock() + seconds * CLOCKS_PER_SEC);
	while(clock() < endwait) {}
}


// From The Code Project http://www.codeproject.com/Articles/1088/Wildcard-string-compare-globbing
// Written by Jack Handy - jakkhandy@hotmail.com
// Modified to compare case insensitive for Windows
int ASConsole::WildCmp(const char* wild, const char* data) const
{
	const char* cp = NULL, *mp = NULL;
	bool cmpval;
	while((*data) && (*wild != '*'))
	{
		if(!g_isCaseSensitive)
			cmpval = (tolower(*wild) != tolower(*data)) && (*wild != '?');
		else
			cmpval = (*wild != *data) && (*wild != '?');
		if(cmpval)
			return 0;
		wild++;
		data++;
	}
	while(*data)
	{
		if(*wild == '*')
		{
			if(!*++wild)
				return 1;
			mp = wild;
			cp = data + 1;
		}
		else
		{
			if(!g_isCaseSensitive)
				cmpval = (tolower(*wild) == tolower(*data) || (*wild == '?'));
			else
				cmpval = (*wild == *data) || (*wild == '?');
			if(cmpval)
			{
				wild++;
				data++;
			}
			else
			{
				wild = mp;
				data = cp++;
			}
		}
	}
	while(*wild == '*')
		wild++;
	return !*wild;
}

//显示最后的系统调用错误
void ASConsole::DisplayLastError()
{
	LPSTR msgBuf;
	DWORD lastError = GetLastError();
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				   NULL, lastError,
				   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // Default language
				   (LPSTR) &msgBuf, 0, NULL);
	// Display the string.
	(*m_lpErrorOut) << "Error (" << lastError << ") " << msgBuf << endl;
	// Free the buffer.
	LocalFree(msgBuf);
}

/**
 * 获得当前目录
 * NOTE: getenv("CD") does not work for Windows Vista.
 *        The Windows function GetCurrentDirectory is used instead.
 *
 * @return              The path of the current directory
 */
string ASConsole::GetCurDirectory(const string &fileName_) const
{
	char currdir[MAX_PATH];
	currdir[0] = '\0';
	if(!GetCurrentDirectoryA(sizeof(currdir), currdir))
		error("Cannot find file", fileName_.c_str());
	return string(currdir);
}

/**
 * WINDOWS function to resolve wildcards and recurse into sub directories.
 * The fileName vector is filled with the path and names of files to process.
 *
 * @param directory     The path of the directory to be processed.
 * @param wildcard      The wildcard to be processed (e.g. *.cpp).
 */
void ASConsole::GetFileListInDirectory(const string &strDirectory, const string &strWildcard)
{
	WIN32_FIND_DATAA findFileData;   // for FindFirstFile and FindNextFile
	// Find the first file in the directory
	// Find will get at least "." and "..".
	string strFindFile = strDirectory + "\\*";
	HANDLE hFind = FindFirstFileA(strFindFile.c_str(), &findFileData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		// Error (3) The system cannot find the path specified.
		// Error (123) The filename, directory name, or volume label syntax is incorrect.
		// ::FindClose(hFind); before exiting
		DisplayLastError();
		error(_("Cannot open directory"), strDirectory.c_str());
	}
	// save files and sub directories
	vector<string> listSubDirectory;    //子目录列表
	do
	{
		// 忽略隐藏或者只读文件
		if(findFileData.cFileName[0] == '.'
		   || (findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
		   || (findFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY))
			continue;
		if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// 是子目录
			if(isRecursive)
			{
				// if a sub directory and recursive, save sub directory
				string strSubDirectoryPath = strDirectory + g_fileSeparator + findFileData.cFileName;
				if(IsExcluedDirectory(strSubDirectoryPath))
					printMsg(_("Exclude Directory  %s\n"), strSubDirectoryPath.substr(mainDirectoryLength));
				else
					listSubDirectory.push_back(strSubDirectoryPath);
			}
		}
		else
		{
			// save the file name
			string strFilePath = strDirectory + g_fileSeparator + findFileData.cFileName;
			// check exclude before wildcmp to avoid "unmatched exclude" error
			bool isExcluded = IsExcluedFileName(findFileData.cFileName);
			// save file name if wildcard match
			if(WildCmp(strWildcard.c_str(), findFileData.cFileName))
			{
				if(isExcluded)
					printMsg(_("Exclude File  %s\n"), strFilePath.substr(mainDirectoryLength));
				else
					fileName.push_back(strFilePath);
			}
		}
	}
	while(FindNextFileA(hFind, &findFileData) != 0);
	::FindClose(hFind);
	// check for processing error
	DWORD dwError = GetLastError();
	if(dwError != ERROR_NO_MORE_FILES)
		error("Error processing directory", strDirectory.c_str());
	//递归搜索子目录
	for(unsigned i = 0; i < listSubDirectory.size(); i++)
		GetFileListInDirectory(listSubDirectory[i], strWildcard);
	return;
}

/**
 * WINDOWS function to format a number according to the current locale.
 * This formats positive integers only, no float.
 *
 * @param num       The number to be formatted.
 * @param lcid      The LCID of the locale to be used for testing.
 * @return          The formatted number.
 */
string ASConsole::GetNumFormat(int num, size_t lcid) const
{
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__BORLANDC__) || defined(__GNUC__)
	// Compilers that don't support C++ locales should still support this assert.
	// The C locale should be set but not the C++.
	// This function is not necessary if the C++ locale is set.
	// The locale().name() return value is not portable to all compilers.
	assert(locale().name() == "C");
#endif
	// convert num to a string
	stringstream alphaNum;
	alphaNum << num;
	string number = alphaNum.str();
	if(useAscii)
		return number;
	// format the number using the Windows API
	if(lcid == 0)
		lcid = LOCALE_USER_DEFAULT;
	int outSize = ::GetNumberFormatA(lcid, 0, number.c_str(), NULL, NULL, 0);
	char* outBuf = new(nothrow) char[outSize];
	if(outBuf == NULL)
		return number;
	::GetNumberFormatA(lcid, 0, number.c_str(), NULL, outBuf, outSize);
	string formattedNum(outBuf);
	delete [] outBuf;
	// remove the decimal
	int decSize = ::GetLocaleInfo(lcid, LOCALE_SDECIMAL, NULL, 0);
	char* decBuf = new(nothrow) char[decSize];
	if(decBuf == NULL)
		return number;
	::GetLocaleInfoA(lcid, LOCALE_SDECIMAL, decBuf, decSize);
	size_t i = formattedNum.rfind(decBuf);
	delete [] decBuf;
	if(i != string::npos)
		formattedNum.erase(i);
	if(!formattedNum.length())
		formattedNum = "0";
	return formattedNum;
}


}   // end of astyle namespace
