#pragma once

//#pragma warning(disable: 4996)  // secure version deprecation warnings
//#pragma warning(disable: 4267)  // 64 bit signed/unsigned loss of data
#include <ctime>

#include <iostream>     // for cout cerr
#include <sstream>

#include "ASFormatter.h"
#include "AStyle_FileEncoding.h"
#include "Utf8_16.h"

#include "ASLocalizer.h"
#include <tchar.h>

#define _(a)	m_localizer.settext(a)

#define __UNIT_TEST__

namespace AStyle
{

//----------------------------------------------------------------------------
// ASConsole class for console build
//----------------------------------------------------------------------------

class ASConsole
{
	public:
		ASConsole();
		virtual ~ASConsole(void) {}
	private:
		ASConsole &operator=(ASConsole &);         // not to be implemented

	public:
		//ִ�к����ӿ�
		int DoMain(int argc, _TCHAR* argv[]);

		//�����������ӿ�
		void SetErrorOut(ostream* lpOut)
		{
			m_lpErrorOut = lpOut;
		}
		void SetExcludeVector(string suffixParam, bool bFnD);
		void SetIsVerbose(bool state)
		{
			m_bIsVerbose = state;
		}

		void convertLineEnds(ostringstream &out, int lineEnd);
		FileEncoding detectEncoding(const char* data, size_t dataSize) const;
		bool getFilesAreIdentical() const;
		int  getFilesFormatted() const;
		bool getIgnoreExcludeErrors() const;
		bool getIgnoreExcludeErrorsDisplay() const;
		bool getIsDryRun() const;
		bool getIsFormattedOnly() const;
		bool getIsQuiet() const;
		bool getIsRecursive() const;
		bool getLineEndsMixed() const;
		bool getNoBackup() const;
		bool getPreserveDate() const;

		//��Ԫ�����ú���
#ifdef __UNIT_TEST__
		string getLanguageID() const
		{
			return m_localizer.getLanguageID();
		}
		bool getIsVerbose() const
		{
			return m_bIsVerbose;
		}

		vector<string> getFileName() const
		{
			return fileName;
		}
		vector<string> getInputFilePathsList() const
		{
			return m_listInputFilePaths;
		}
		vector<string> getExcludeList() const
		{
			return m_listExclude;
		}
		vector<bool> getExcludeFnDList() const
		{
			return m_listExcludeFnD;
		}
		vector<bool> getExcludeHitsVector() const
		{
			return excludeHitsVector;
		}
#endif

		void setIsQuiet(bool state);
		void setIsRecursive(bool state);

		string getNumberFormat(int num, const char* groupingArg, const char* separator) const;
		string getOrigSuffix() const;
		void setBypassBrowserOpen(bool state);
		void setIgnoreExcludeErrors(bool state);
		void setIgnoreExcludeErrorsAndDisplay(bool state);
		void setIsDryRun(bool state);
		void setIsFormattedOnly(bool state);
		void setNoBackup(bool state);
		void setOrigSuffix(string suffix);
		void setPreserveDate(bool state);
		void standardizePath(string &path, bool removeBeginningSeparator = false) const;
		bool stringEndsWith(const string &str, const string &suffix) const;

	private:
		//������ӿ�
		void error() const;
		void error(const char* why, const char* what) const;

		//���߰���ϵͳ�ӿ�
		void PrintHelp() const;

		//��Ϣ��ʾ��ؽӿ�
		void PrintSeparatingLine(int nNum = 60) const;
		clock_t PrintVerboseHeader() const;
		void PrintVerboseStats(clock_t startTime) const;

		//���߸�������
		size_t SplitFilePath(string &strFilePath,string &strFileDirectory,string &strFileName);
		int WildCmp(const char* wild, const char* data) const;

		//���������ú���
		void GetArgvOptions(int argc, _TCHAR* argv[], vector<string>& listOptions) const;
		void ProcessOptions(vector<string> &argvOptions);
		//�����ļ���ؽӿ�
		const char* GetOptionsFileName() const
		{
			return m_strOptionsFileName.c_str();
		}
		void SetOptionsFileName(string name)
		{
			m_strOptionsFileName = name;
		}
		bool OptionsFileNameIsEmpty() const
		{
			return (m_strOptionsFileName.compare("") == 0);
		}
		void InitOptionsFileName(void);

		//ת������ӿ�
		void ProcessFiles();
		void ProcessInputFilePaths(string &filePath);
		bool IsExcluedDirectory(const string &strDirectoryPath);
		bool IsExcluedFileName(const string &strFileName);

		void formatCinToCout();

		//ƽ̨��غ���
		void LaunchDefaultBrowser(const char* filePathIn = NULL) const;
		string GetCurDirectory(const string &fileName_) const;
		void GetFileListInDirectory(const string &strDirectory, const string &strWildcard);

		string GetNumFormat(int num, size_t lcid = 0) const;
#ifdef _WIN32
		void DisplayLastError();
#endif
		//�ļ�������غ���
		int waitForRemove(const char* oldFileName) const;
		void writeFile(const string &fileName_, FileEncoding encoding, ostringstream &out) const;

		void correctMixedLineEnds(ostringstream &out);
		void formatFile(const string &fileName_);
		void initializeOutputEOL(LineEndFormat lineEndFormat);
		void printMsg(const char* msg, const string &data) const;
		FileEncoding readFile(const string &fileName_, stringstream &in) const;
		void removeFile(const char* fileName_, const char* errMsg) const;
		void renameFile(const char* oldFileName, const char* newFileName, const char* errMsg) const;
		void setOutputEOL(LineEndFormat lineEndFormat, const char* currentEOL);
		void sleep(int seconds) const;

	private:
		ostream* m_lpErrorOut;
		ASFormatter m_Formatter;            // ASFormatter object
		ASLocalizer m_localizer;            // ASLocalizer object
		Utf8_16 utf8_16;                    // utf8/16 conversion methods

		vector<string> m_listExclude;       // ��Ҫ�ų���Ŀ¼���ͺ�׺�б�
		vector<bool> m_listExcludeFnD;		// �ų���ΪĿ¼�����ļ�������б�
		vector<bool> excludeHitsVector;     // exclude flags for error reporting
		vector<string> m_listInputFilePaths; // �����������ļ�·���б�
		vector<string> fileName;            // files to be processed including path


		//�����в���
		bool m_bIsVerbose;                  // verboseѡ��Ƿ���ʾ�����ͳ����Ϣ

		bool isRecursive;                   // recursive option
		bool isDryRun;                      // dry-run option
		bool noBackup;                      // suffix=none option
		bool preserveDate;                  // preserve-date option
		bool isQuiet;                       // quiet option
		bool isFormattedOnly;               // formatted lines only option
		bool ignoreExcludeErrors;           // don't abort on unmatched excludes
		bool ignoreExcludeErrorsDisplay;    // don't display unmatched excludes
		bool useAscii;                      // ascii option

		//��������
		bool bypassBrowserOpen;             // don't open the browser on html options
		bool hasWildcard;                   // file name includes a wildcard
		bool filesAreIdentical;             // input and output files are identical
		bool lineEndsMixed;                 // output has mixed line ends

		size_t mainDirectoryLength;         // directory length to be excluded in displays
		int filesFormatted;                // number of files formatted
		int filesUnchanged;                // number of files unchanged
		int linesOut;                      // number of output lines

		char outputEOL[4];                  // current line end
		char prevEOL[4];                    // previous line end
		string origSuffix;                  // suffix= option

		string m_strOptionsFileName;        // file path and name of the options file to use
		string targetDirectory;             // path to the directory being processed
		string targetFilename;              // file name being processed

	private:
		static const char* g_strVersion;
		static char g_fileSeparator;
		static bool g_isCaseSensitive;
};


}   // end of astyle namespace
