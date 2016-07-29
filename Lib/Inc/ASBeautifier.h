#pragma once

#include "ASSourceIterator.h"
#include "ASResource.h"
#include "ASBase.h"

namespace AStyle
{

enum MinConditional
{
	MINCOND_ZERO,
	MINCOND_ONE,
	MINCOND_TWO,
	MINCOND_ONEHALF,
	MINCOND_END
};

//-----------------------------------------------------------------------------
// Class ASBeautifier
//-----------------------------------------------------------------------------

class ASBeautifier : protected ASResource, protected ASBase
{
	public:
		ASBeautifier(void);
		virtual ~ASBeautifier(void);
		virtual void init(ASSourceIterator* iter);
		virtual string beautify(const string &line);
		void setCaseIndent(bool state);
		void setClassIndent(bool state);
		void setCStyle();
		void setDefaultTabLength();
		void setEmptyLineFill(bool state);
		void setForceTabXIndentation(int length);
		void setJavaStyle();
		void setLabelIndent(bool state);
		void setMaxInStatementIndentLength(int max);
		void setMinConditionalIndentOption(int min);
		void setMinConditionalIndentLength();
		void setModeManuallySet(bool state);
		void setModifierIndent(bool state);
		void setNamespaceIndent(bool state);
		void setAlignMethodColon(bool state);
		void setSharpStyle();
		void setSpaceIndentation(int length = 4);
		void setSwitchIndent(bool state);
		void setTabIndentation(int length = 4, bool forceTabs = false);
		void setPreprocDefineIndent(bool state);
		void setPreprocConditionalIndent(bool state);
		int  getBeautifierFileType() const;
		int  getFileType() const;
		int  getIndentLength(void) const;
		int  getTabLength(void) const;
		string getIndentString(void) const;
		string getNextWord(const string &line, size_t currPos) const;
		bool getBracketIndent(void) const;
		bool getBlockIndent(void) const;
		bool getCaseIndent(void) const;
		bool getClassIndent(void) const;
		bool getEmptyLineFill(void) const;
		bool getForceTabIndentation(void) const;
		bool getModeManuallySet(void) const;
		bool getModifierIndent(void) const;
		bool getNamespaceIndent(void) const;
		bool getPreprocDefineIndent(void) const;
		bool getSwitchIndent(void) const;

	protected:
		void deleteBeautifierVectors();
		const string* findHeader(const string &line, int i,
								 const vector<const string*>* possibleHeaders) const;
		const string* findOperator(const string &line, int i,
								   const vector<const string*>* possibleOperators) const;
		int  getNextProgramCharDistance(const string &line, int i) const;
		int  indexOf(vector<const string*> &container, const string* element) const;
		void setBlockIndent(bool state);
		void setBracketIndent(bool state);
		void setBracketIndentVtk(bool state);
		string extractPreprocessorStatement(const string &line) const;
		string trim(const string &str) const;
		string rtrim(const string &str) const;

	private:  // functions
		ASBeautifier(const ASBeautifier &copy);
		ASBeautifier &operator=(ASBeautifier &);       // not to be implemented

		void adjustParsedLineIndentation(size_t iPrelim, bool isInExtraHeaderIndent);
		void computePreliminaryIndentation();
		void parseCurrentLine(const string &line);
		void popLastInStatementIndent();
		void processPreprocessor(const string &preproc, const string &line);
		void registerInStatementIndent(const string &line, int i, int spaceIndentCount,
									   int tabIncrementIn, int minIndent, bool updateParenStack);
		void registerInStatementIndentColon(const string &line, int i, int tabIncrementIn);
		void initVectors();
		void initTempStacksContainer(vector<vector<const string*>*>* &container,
									 vector<vector<const string*>*>* value);
		void clearObjCMethodDefinitionAlignment();
		void deleteBeautifierContainer(vector<ASBeautifier*>* &container);
		void deleteTempStacksContainer(vector<vector<const string*>*>* &container);
		int  adjustIndentCountForBreakElseIfComments() const;
		int  computeObjCColonAlignment(string &line, int colonAlignPosition) const;
		int  convertTabToSpaces(int i, int tabIncrementIn) const;
		int  getInStatementIndentAssign(const string &line, size_t currPos) const;
		int  getInStatementIndentComma(const string &line, size_t currPos) const;
		bool isIndentedPreprocessor(const string &line, size_t currPos) const;
		bool isLineEndComment(const string &line, int startPos) const;
		bool isPreprocessorConditionalCplusplus(const string &line) const;
		bool isInPreprocessorUnterminatedComment(const string &line);
		bool statementEndsWithComma(const string &line, int index) const;
		string &getIndentedLineReturn(string &newLine, const string &originalLine) const;
		string preLineWS(int lineIndentCount, int lineSpaceIndentCount) const;
		template<typename T> void deleteContainer(T &container);
		template<typename T> void initContainer(T &container, T value);
		vector<vector<const string*>*>* copyTempStacks(const ASBeautifier &other) const;
		pair<int, int> computePreprocessorIndent();

	protected:
		// variables set by ASFormatter - must be updated in activeBeautifierStack
		int  inLineNumber;
		int  horstmannIndentInStatement;
		int  nonInStatementBracket;
		bool lineCommentNoBeautify;
		bool isElseHeaderIndent;
		bool isCaseHeaderCommentIndent;
		bool isNonInStatementArray;
		bool isSharpAccessor;
		bool isSharpDelegate;
		bool isInExternC;
		bool isInBeautifySQL;
		bool isInIndentableStruct;
		bool isInIndentablePreproc;

	private:  // variables
		int beautifierFileType;
		vector<const string*>* headers;
		vector<const string*>* nonParenHeaders;
		vector<const string*>* preBlockStatements;
		vector<const string*>* preCommandHeaders;
		vector<const string*>* assignmentOperators;
		vector<const string*>* nonAssignmentOperators;
		vector<const string*>* indentableHeaders;

		vector<ASBeautifier*>* waitingBeautifierStack;
		vector<ASBeautifier*>* activeBeautifierStack;
		vector<int>* waitingBeautifierStackLengthStack;
		vector<int>* activeBeautifierStackLengthStack;
		vector<const string*>* headerStack;
		vector<vector<const string*>* >* tempStacks;
		vector<int>* blockParenDepthStack;
		vector<bool>* blockStatementStack;
		vector<bool>* parenStatementStack;
		vector<bool>* bracketBlockStateStack;
		vector<int>* inStatementIndentStack;
		vector<int>* inStatementIndentStackSizeStack;
		vector<int>* parenIndentStack;
		vector<pair<int, int> >* preprocIndentStack;

		ASSourceIterator* sourceIterator;
		const string* currentHeader;
		const string* previousLastLineHeader;
		const string* probationHeader;
		const string* lastLineHeader;
		string indentString;
		string verbatimDelimiter;
		bool isInQuote;
		bool isInVerbatimQuote;
		bool haveLineContinuationChar;
		bool isInAsm;
		bool isInAsmOneLine;
		bool isInAsmBlock;
		bool isInComment;
		bool isInPreprocessorComment;
		bool isInHorstmannComment;
		bool isInCase;
		bool isInQuestion;
		bool isInStatement;
		bool isInHeader;
		bool isInTemplate;
		bool isInDefine;
		bool isInDefineDefinition;
		bool classIndent;
		bool isIndentModeOff;
		bool isInClassHeader;           // is in a class before the opening bracket
		bool isInClassHeaderTab;        // is in an indentable class header line
		bool isInClassInitializer;      // is in a class after the ':' initializer
		bool isInClass;                 // is in a class after the opening bracket
		bool isInObjCMethodDefinition;
		bool isImmediatelyPostObjCMethodDefinition;
		bool isInIndentablePreprocBlock;
		bool isInObjCInterface;
		bool isInEnum;
		bool isInEnumTypeID;
		bool isInLet;
		bool modifierIndent;
		bool switchIndent;
		bool caseIndent;
		bool namespaceIndent;
		bool bracketIndent;
		bool bracketIndentVtk;
		bool blockIndent;
		bool labelIndent;
		bool shouldIndentPreprocDefine;
		bool isInConditional;
		bool isModeManuallySet;
		bool shouldForceTabIndentation;
		bool emptyLineFill;
		bool backslashEndsPrevLine;
		bool lineOpensWithLineComment;
		bool lineOpensWithComment;
		bool lineStartsInComment;
		bool blockCommentNoIndent;
		bool blockCommentNoBeautify;
		bool previousLineProbationTab;
		bool lineBeginsWithOpenBracket;
		bool lineBeginsWithCloseBracket;
		bool lineBeginsWithComma;
		bool lineIsCommentOnly;
		bool lineIsLineCommentOnly;
		bool shouldIndentBrackettedLine;
		bool isInSwitch;
		bool foundPreCommandHeader;
		bool foundPreCommandMacro;
		bool shouldAlignMethodColon;
		bool shouldIndentPreprocConditional;
		int  indentCount;
		int  spaceIndentCount;
		int  spaceIndentObjCMethodDefinition;
		int  colonIndentObjCMethodDefinition;
		int  lineOpeningBlocksNum;
		int  lineClosingBlocksNum;
		int  fileType;
		int  minConditionalOption;
		int  minConditionalIndent;
		int  parenDepth;
		int  indentLength;
		int  tabLength;
		int  blockTabCount;
		int  maxInStatementIndent;
		int  classInitializerIndents;
		int  templateDepth;
		int  squareBracketCount;
		int  prevFinalLineSpaceIndentCount;
		int  prevFinalLineIndentCount;
		int  defineIndentCount;
		int  preprocBlockIndent;
		char quoteChar;
		char prevNonSpaceCh;
		char currentNonSpaceCh;
		char currentNonLegalCh;
		char prevNonLegalCh;
};

}   // end of astyle namespace
