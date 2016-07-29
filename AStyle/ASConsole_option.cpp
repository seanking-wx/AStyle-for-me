#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的
#include <AtlBase.h>

#include <fstream>

#include "ASOptions.h"
namespace AStyle
{
// build a vector of argv options
// the program path argv[0] is excluded
void ASConsole::GetArgvOptions(int argc, _TCHAR* argv[], vector<string>& listOptions) const
{
	listOptions.clear();
	for(int i = 1; i < argc; i++)
		listOptions.push_back(string(CT2A(argv[i])));
}

// process options from the command line and options file
// build the vectors fileNameVector, excludeVector, optionsVector, and fileOptionsVector
void ASConsole::ProcessOptions(vector<string> &argvOptions)
{
	vector<string> listOptions;       // options from the command line
	bool bShouldParseOptionsFile = true;
	bool bOptionsFileRequired = false;
	m_strOptionsFileName.clear();
	// get command line options
	for(size_t i = 0; i < argvOptions.size(); i++)
	{
		string arg = argvOptions[i];
		if(ASOptions::IsOption3(arg, "-h", "-?", "--help"))
		{
			PrintHelp();
			return;
		}
		else if(ASOptions::IsOption2(arg, "-!", "--html"))
		{
			LaunchDefaultBrowser();
			return;
		}
		else if(ASOptions::IsParamOption(arg, "--html="))
		{
			string htmlFilePath = ASOptions::GetParam(arg, "--html=");
			LaunchDefaultBrowser(htmlFilePath.c_str());
			return;
		}
		else if(ASOptions::IsOption2(arg, "-V", "--version"))
		{
			printf("Artistic Style Version %s\n", g_strVersion);
			exit(EXIT_SUCCESS);
			return;
		}
		else if(ASOptions::IsOption2(arg, "-I", "--ascii"))
		{
			useAscii = true;
			setlocale(LC_ALL, "C");     // use English decimal indicator
			m_localizer.setLanguageFromName("en");
		}
		else if(ASOptions::IsOption(arg, "--options=none"))
			bShouldParseOptionsFile = false;
		else if(ASOptions::IsParamOption(arg, "--options="))
		{
			SetOptionsFileName(ASOptions::GetParam(arg, "--options="));
			bOptionsFileRequired = true;
			if(OptionsFileNameIsEmpty())
				SetOptionsFileName(" ");        //为了产生"Cannot open options file"的错误,否则不会报错，等效于该参数不存在
		}
		else if(arg[0] == '-')
			listOptions.push_back(arg);
		else // file-name
		{
			standardizePath(arg);
			m_listInputFilePaths.push_back(arg);
		}
	}
	// get options file path and name
	if(bShouldParseOptionsFile)
		InitOptionsFileName();
	// create the options file vector and parse the options for errors
	ASOptions options(*this,m_Formatter);
	bool ok = true;
	if(!OptionsFileNameIsEmpty())
	{
		ifstream fileOptions(GetOptionsFileName());
		if(fileOptions)
			ok = options.ParseOptions(fileOptions, string(_("Invalid option file options:")));
		else if(bOptionsFileRequired)
		{
			error(_("Cannot open options file"), GetOptionsFileName());
			return;
		}
		fileOptions.close();
	}
	if(ok)  // parse the command line options vector for errors
		ok = options.ParseOptions(listOptions, string(_("Invalid command line options:")));
	if(!ok)
	{
		(*m_lpErrorOut) << options.GetOptionErrors() << endl;
		(*m_lpErrorOut) << _("For help on options type 'astyle -h'") << endl;
		error();
	}
}
void ASConsole::InitOptionsFileName(void)
{
	if(OptionsFileNameIsEmpty())
	{
		char* env = getenv("ARTISTIC_STYLE_OPTIONS");
		if(env != NULL)
			SetOptionsFileName(env);
	}
	if(OptionsFileNameIsEmpty())
	{
		char* env = getenv("ASTYLE_PATH");
		if(env != NULL)
			SetOptionsFileName(string(env) + "/astyle.rc");
	}
	if(OptionsFileNameIsEmpty())
	{
		char* env = getenv("HOME");
		if(env != NULL)
			SetOptionsFileName(string(env) + "/.astylerc");
	}
	if(OptionsFileNameIsEmpty())
	{
		char* env = getenv("USERPROFILE");
		if(env != NULL)
			SetOptionsFileName(string(env) + "/astylerc");
	}
	if(!OptionsFileNameIsEmpty())
		standardizePath(m_strOptionsFileName);
}

}   // end of astyle namespace
