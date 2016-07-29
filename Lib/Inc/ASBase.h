#pragma once
#include <cassert>
#include <string>
#include "AStyle_FileType.h"

namespace AStyle
{

using namespace std;

//-----------------------------------------------------------------------------
// Class ASBase
//-----------------------------------------------------------------------------

class ASBase
{
	private:
		// all variables should be set by the "init" function
		int baseFileType;      // a value from enum FileType

	protected:
		ASBase() : baseFileType(C_TYPE) { }
		virtual ~ASBase() {}

		// functions definitions are at the end of ASResource.cpp
		bool findKeyword(const string &line, int i, const string &keyword) const;
		string getCurrentWord(const string &line, size_t index) const;

	protected:
		void init(int fileTypeArg)
		{
			baseFileType = fileTypeArg;
		}
		bool isCStyle() const
		{
			return (baseFileType == C_TYPE);
		}
		bool isJavaStyle() const
		{
			return (baseFileType == JAVA_TYPE);
		}
		bool isSharpStyle() const
		{
			return (baseFileType == SHARP_TYPE);
		}

		// check if a specific character is a digit
		// NOTE: Visual C isdigit() gives assert error if char > 256
		bool isDigit(char ch) const
		{
			return (ch >= '0' && ch <= '9');
		}

		// check if a specific character can be used in a legal variable/method/class name
		bool isLegalNameChar(char ch) const
		{
			if(isWhiteSpace(ch)) return false;
			if((unsigned) ch > 127) return false;
			return (isalnum((unsigned char)ch)
					|| ch == '.' || ch == '_'
					|| (isJavaStyle() && ch == '$')
					|| (isSharpStyle() && ch == '@'));  // may be used as a prefix
		}

		// check if a specific character can be part of a header
		bool isCharPotentialHeader(const string &line, size_t i) const
		{
			assert(!isWhiteSpace(line[i]));
			char prevCh = ' ';
			if(i > 0) prevCh = line[i - 1];
			if(!isLegalNameChar(prevCh) && isLegalNameChar(line[i]))
				return true;
			return false;
		}

		// check if a specific character can be part of an operator
		bool isCharPotentialOperator(char ch) const
		{
			assert(!isWhiteSpace(ch));
			if((unsigned) ch > 127) return false;
			return (ispunct((unsigned char)ch)
					&& ch != '{' && ch != '}'
					&& ch != '(' && ch != ')'
					&& ch != '[' && ch != ']'
					&& ch != ';' && ch != ','
					&& ch != '#' && ch != '\\'
					&& ch != '\'' && ch != '\"');
		}

		// check if a specific character is a whitespace character
		bool isWhiteSpace(char ch) const
		{
			return (ch == ' ' || ch == '\t');
		}

		// peek at the next unread character.
		char peekNextChar(const string &line, int i) const
		{
			char ch = ' ';
			size_t peekNum = line.find_first_not_of(" \t", i + 1);
			if(peekNum == string::npos)
				return ch;
			ch = line[peekNum];
			return ch;
		}
};

}   // end of astyle namespace
