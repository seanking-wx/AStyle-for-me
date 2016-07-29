#pragma once

#include <vector>
#include "ASBase.h"

namespace AStyle
{

//-----------------------------------------------------------------------------
// Class ASEnhancer
//-----------------------------------------------------------------------------

class ASEnhancer : protected ASBase
{
	private:
		// struct used by ParseFormattedLine function
		// contains variables used to unindent the case blocks
		struct switchVariables
		{
			int  switchBracketCount;
			int  unindentDepth;
			bool unindentCase;
		};

	public:
		ASEnhancer(void) {}
		virtual ~ASEnhancer(void) {}
		void init(int  _fileType,
				  int  _indentLength,
				  int  _tabLength,
				  bool _useTabs,
				  bool _forceTab,
				  bool _namespaceIndent,
				  bool _caseIndent,
				  bool _preprocBlockIndent,
				  bool _preprocDefineIndent,
				  bool _emptyLineFill,
				  vector<const pair<const string, const string>* >* _indentableMacros);
		void enhance(string &line, bool isInNamespace, bool isInPreprocessor, bool isInSQL);

	private:
		void    convertForceTabIndentToSpaces(string  &line) const;
		void    convertSpaceIndentToForceTab(string &line) const;
		size_t  findCaseColon(string  &line, size_t caseIndex) const;
		int     indentLine(string  &line, int indent) const;
		bool    isBeginDeclareSectionSQL(string  &line, size_t index) const;
		bool    isEndDeclareSectionSQL(string  &line, size_t index) const;
		bool    isOneLineBlockReached(string &line, int startChar) const;
		void    parseCurrentLine(string &line, bool isInPreprocessor, bool isInSQL);
		size_t  processSwitchBlock(string  &line, size_t index);
		int     unindentLine(string  &line, int unindent) const;

	private:
		// options from command line or options file
		int  indentLength;
		int  tabLength;
		bool useTabs;
		bool forceTab;
		bool namespaceIndent;
		bool caseIndent;
		bool preprocBlockIndent;
		bool preprocDefineIndent;
		bool emptyLineFill;

		// parsing variables
		int  lineNumber;
		bool isInQuote;
		bool isInComment;
		char quoteChar;

		// unindent variables
		int  bracketCount;
		int  switchDepth;
		int  eventPreprocDepth;
		bool lookingForCaseBracket;
		bool unindentNextLine;
		bool shouldUnindentLine;
		bool shouldUnindentComment;

		switchVariables sw;                      // switch variables struct
		vector<switchVariables> switchStack;     // stack vector of switch variables

		// event table variables
		bool nextLineIsEventIndent;             // begin event table indent is reached
		bool isInEventTable;                    // need to indent an event table
		vector<const pair<const string, const string>* >* indentableMacros;

		// SQL variables
		bool nextLineIsDeclareIndent;           // begin declare section indent is reached
		bool isInDeclareSection;                // need to indent a declare section
};

}   // end of astyle namespace
