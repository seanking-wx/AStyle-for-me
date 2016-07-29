#pragma once
#include "ASConsole.h"

namespace AStyle
{

//----------------------------------------------------------------------------
// ASOptions class for options processing
// used by both console and library builds
//----------------------------------------------------------------------------

class ASOptions
{
	public:
		ASOptions(ASConsole &console, ASFormatter &formatterArg) :
			m_Console(console), m_Formatter(formatterArg)
		{
		}
		virtual ~ASOptions(void) {}
	private:
		ASOptions &operator=(ASOptions &);         // not to be implemented

	public:
		bool ParseOptions(istream &in, const string &errorInfo);
		bool ParseOptions(vector<string> &optionsVector, const string &errorInfo);
		string GetOptionErrors() const
		{
			return m_Error.str();
		}

	private:
		//参数文件分析
		void ImportOptions(istream &in, vector<string> &optionsVector);
		//参数项处理
		void ParseOption(const string &arg, const string &errorInfo);
		void IsOptionError(const string &arg, const string &errorInfo)
		{
			if(m_Error.str().length() == 0)
				m_Error << errorInfo << endl;   // need main error message
			m_Error << arg << endl;
		}

	private:
		ASConsole &m_Console;           // reference to the ASConsole object
		ASFormatter &m_Formatter;       // reference to the ASFormatter object
		stringstream m_Error;           // option error messages

	public:
		static bool IsOption(const string &arg, const char* op)
		{
			return arg.compare(op) == 0;
		}
		static bool IsOption2(const string &arg, const char* op1, const char* op2)
		{
			return (IsOption(arg, op1) || IsOption(arg, op2));
		}
		static bool IsOption3(const string &arg, const char* op1, const char* op2, const char* op3)
		{
			return (IsOption(arg, op1) || IsOption(arg, op2) || IsOption(arg, op2));
		}

		static bool IsParamOption(const string &arg, const char* option)
		{
			bool retVal = arg.compare(0, strlen(option), option) == 0;
			// if comparing for short option, 2nd char of arg must be numeric
			if(retVal && strlen(option) == 1 && arg.length() > 1)
			{
				if(!isdigit((unsigned char)arg[1]))
					retVal = false;
			}
			return retVal;
		}
		static bool IsParamOption2(const string &arg, const char* option1, const char* option2)
		{
			return IsParamOption(arg, option1) || IsParamOption(arg, option2);
		}

		static string GetParam(const string &arg, const char* op)
		{
			return arg.substr(strlen(op));
		}
		static string GetParam2(const string &arg, const char* op1, const char* op2)
		{
			return IsParamOption(arg, op1) ? GetParam(arg, op1) : GetParam(arg, op2);
		}

};

}   // end of astyle namespace
