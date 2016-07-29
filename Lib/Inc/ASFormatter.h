#pragma once

#include "ASBeautifier.h"
#include "ASEnhancer.h"
//#include "AStyle_enum.h"
namespace AStyle
{

enum LineEndFormat
{
	LINEEND_DEFAULT,    // Use line break that matches most of the file
	LINEEND_WINDOWS,
	LINEEND_LINUX,
	LINEEND_MACOLD,
	LINEEND_CRLF = LINEEND_WINDOWS,
	LINEEND_LF   = LINEEND_LINUX,
	LINEEND_CR   = LINEEND_MACOLD
};

/* The enums below are not recognized by 'vectors' in Microsoft Visual C++
   V5 when they are part of a namespace!!!  Use Visual C++ V6 or higher.
*/
enum FormatStyle
{
	STYLE_NONE,
	STYLE_ALLMAN,
	STYLE_JAVA,
	STYLE_KR,
	STYLE_STROUSTRUP,
	STYLE_WHITESMITH,
	STYLE_VTK,
	STYLE_BANNER,
	STYLE_GNU,
	STYLE_LINUX,
	STYLE_HORSTMANN,
	STYLE_1TBS,
	STYLE_GOOGLE,
	STYLE_PICO,
	STYLE_LISP
};

enum BracketMode
{
	NONE_MODE,
	ATTACH_MODE,
	BREAK_MODE,
	LINUX_MODE,
	STROUSTRUP_MODE,
	RUN_IN_MODE
};

enum ObjCColonPad
{
	COLON_PAD_NO_CHANGE,
	COLON_PAD_NONE,
	COLON_PAD_ALL,
	COLON_PAD_AFTER,
	COLON_PAD_BEFORE
};

enum PointerAlign
{
	PTR_ALIGN_NONE,
	PTR_ALIGN_TYPE,
	PTR_ALIGN_MIDDLE,
	PTR_ALIGN_NAME
};

enum ReferenceAlign
{
	REF_ALIGN_NONE = PTR_ALIGN_NONE,
	REF_ALIGN_TYPE = PTR_ALIGN_TYPE,
	REF_ALIGN_MIDDLE = PTR_ALIGN_MIDDLE,
	REF_ALIGN_NAME = PTR_ALIGN_NAME,
	REF_SAME_AS_PTR
};

enum BracketType
{
	NULL_TYPE = 0,
	NAMESPACE_TYPE = 1,         // also a DEFINITION_TYPE
	CLASS_TYPE = 2,             // also a DEFINITION_TYPE
	STRUCT_TYPE = 4,            // also a DEFINITION_TYPE
	INTERFACE_TYPE = 8,         // also a DEFINITION_TYPE
	DEFINITION_TYPE = 16,
	COMMAND_TYPE = 32,
	ARRAY_NIS_TYPE = 64,        // also an ARRAY_TYPE
	ENUM_TYPE = 128,            // also an ARRAY_TYPE
	INIT_TYPE = 256,            // also an ARRAY_TYPE
	ARRAY_TYPE = 512,
	EXTERN_TYPE = 1024,         // extern "C", not a command type extern
	SINGLE_LINE_TYPE = 2048
};

//-----------------------------------------------------------------------------
// Class ASFormatter
//-----------------------------------------------------------------------------

class ASFormatter : public ASBeautifier
{
	public:
		ASFormatter(void);
		virtual ~ASFormatter(void);
		virtual void init(ASSourceIterator* iter);
		virtual bool hasMoreLines() const;
		virtual string nextLine();
		LineEndFormat getLineEndFormat() const;
		bool getIsLineReady() const;
		void setFormattingStyle(FormatStyle style);
		void setAddBracketsMode(bool state);
		void setAddOneLineBracketsMode(bool state);
		void setRemoveBracketsMode(bool state);
		void setAttachClass(bool state);
		void setAttachExternC(bool state);
		void setAttachNamespace(bool state);
		void setAttachInline(bool state);
		void setBracketFormatMode(BracketMode mode);
		void setBreakAfterMode(bool state);
		void setBreakClosingHeaderBracketsMode(bool state);
		void setBreakBlocksMode(bool state);
		void setBreakClosingHeaderBlocksMode(bool state);
		void setBreakElseIfsMode(bool state);
		void setBreakOneLineBlocksMode(bool state);
		void setMethodPrefixPaddingMode(bool state);
		void setMethodPrefixUnPaddingMode(bool state);
		void setCloseTemplatesMode(bool state);
		void setDeleteEmptyLinesMode(bool state);
		void setIndentCol1CommentsMode(bool state);
		void setLineEndFormat(LineEndFormat fmt);
		void setMaxCodeLength(int max);
		void setObjCColonPaddingMode(ObjCColonPad mode);
		void setOperatorPaddingMode(bool mode);
		void setParensOutsidePaddingMode(bool mode);
		void setParensFirstPaddingMode(bool mode);
		void setParensInsidePaddingMode(bool mode);
		void setParensHeaderPaddingMode(bool mode);
		void setParensUnPaddingMode(bool state);
		void setPointerAlignment(PointerAlign alignment);
		void setPreprocBlockIndent(bool state);
		void setReferenceAlignment(ReferenceAlign alignment);
		void setSingleStatementsMode(bool state);
		void setStripCommentPrefix(bool state);
		void setTabSpaceConversionMode(bool state);
		size_t getChecksumIn() const;
		size_t getChecksumOut() const;
		int  getChecksumDiff() const;
		int  getFormatterFileType() const;

	private:  // functions
		ASFormatter(const ASFormatter &copy);       // copy constructor not to be implemented
		ASFormatter &operator=(ASFormatter &);      // assignment operator not to be implemented
		template<typename T> void deleteContainer(T &container);
		template<typename T> void initContainer(T &container, T value);
		char peekNextChar() const;
		BracketType getBracketType();
		bool adjustChecksumIn(int adjustment);
		bool computeChecksumIn(const string &currentLine_);
		bool computeChecksumOut(const string &beautifiedLine);
		bool addBracketsToStatement();
		bool removeBracketsFromStatement();
		bool commentAndHeaderFollows();
		bool getNextChar();
		bool getNextLine(bool emptyLineWasDeleted = false);
		bool isArrayOperator() const;
		bool isBeforeComment() const;
		bool isBeforeAnyComment() const;
		bool isBeforeAnyLineEndComment(int startPos) const;
		bool isBeforeMultipleLineEndComments(int startPos) const;
		bool isBracketType(BracketType a, BracketType b) const;
		bool isClassInitializer() const;
		bool isClosingHeader(const string* header) const;
		bool isCurrentBracketBroken() const;
		bool isDereferenceOrAddressOf() const;
		bool isExecSQL(string &line, size_t index) const;
		bool isEmptyLine(const string &line) const;
		bool isExternC() const;
		bool isNextWordSharpNonParenHeader(int startChar) const;
		bool isNonInStatementArrayBracket() const;
		bool isOkToSplitFormattedLine();
		bool isPointerOrReference() const;
		bool isPointerOrReferenceCentered() const;
		bool isPointerOrReferenceVariable(string &word) const;
		bool isSharpStyleWithParen(const string* header) const;
		bool isStructAccessModified(string &firstLine, size_t index) const;
		bool isIndentablePreprocessorBlock(string &firstLine, size_t index);
		bool isUnaryOperator() const;
		bool isUniformInitializerBracket() const;
		bool isImmediatelyPostCast() const;
		bool isInExponent() const;
		bool isInSwitchStatement() const;
		bool isNextCharOpeningBracket(int startChar) const;
		bool isOkToBreakBlock(BracketType bracketType) const;
		bool isOperatorPaddingDisabled() const;
		bool pointerSymbolFollows() const;
		int  getCurrentLineCommentAdjustment();
		int  getNextLineCommentAdjustment();
		int  isOneLineBlockReached(string &line, int startChar) const;
		void adjustComments();
		void appendChar(char ch, bool canBreakLine);
		void appendCharInsideComments();
		void appendOperator(const string &sequence, bool canBreakLine = true);
		void appendSequence(const string &sequence, bool canBreakLine = true);
		void appendSpacePad();
		void appendSpaceAfter();
		void breakLine(bool isSplitLine = false);
		void buildLanguageVectors();
		void updateFormattedLineSplitPoints(char appendedChar);
		void updateFormattedLineSplitPointsOperator(const string &sequence);
		void checkIfTemplateOpener();
		void clearFormattedLineSplitPoints();
		void convertTabToSpaces();
		void deleteContainer(vector<BracketType>* &container);
		void formatArrayRunIn();
		void formatRunIn();
		void formatArrayBrackets(BracketType bracketType, bool isOpeningArrayBracket);
		void formatClosingBracket(BracketType bracketType);
		void formatCommentBody();
		void formatCommentOpener();
		void formatCommentCloser();
		void formatLineCommentBody();
		void formatLineCommentOpener();
		void formatOpeningBracket(BracketType bracketType);
		void formatQuoteBody();
		void formatQuoteOpener();
		void formatPointerOrReference();
		void formatPointerOrReferenceCast();
		void formatPointerOrReferenceToMiddle();
		void formatPointerOrReferenceToName();
		void formatPointerOrReferenceToType();
		void fixOptionVariableConflicts();
		void goForward(int i);
		void isLineBreakBeforeClosingHeader();
		void initContainer(vector<BracketType>* &container, vector<BracketType>* value);
		void initNewLine();
		void padObjCMethodColon();
		void padOperators(const string* newOperator);
		void padParens();
		void processPreprocessor();
		void resetEndOfStatement();
		void setAttachClosingBracketMode(bool state);
		void setBreakBlocksVariables();
		void stripCommentPrefix();
		void testForTimeToSplitFormattedLine();
		void trimContinuationLine();
		void updateFormattedLineSplitPointsPointerOrReference(size_t index);
		size_t findFormattedLineSplitPoint() const;
		size_t findNextChar(string &line, char searchChar, int searchStart = 0);
		const string* checkForHeaderFollowingComment(const string &firstLine) const;
		const string* getFollowingOperator() const;
		string getPreviousWord(const string &line, int currPos) const;
		string peekNextText(const string &firstLine, bool endOnEmptyLine = false, bool shouldReset = false) const;

		// append the CURRENT character (curentChar) to the current formatted line.
		void appendCurrentChar(bool canBreakLine = true)
		{
			appendChar(currentChar, canBreakLine);
		}

		// check if a specific sequence exists in the current placement of the current line
		bool isSequenceReached(const char* sequence) const
		{
			return currentLine.compare(charNum, strlen(sequence), sequence) == 0;
		}

		// call ASBase::findHeader for the current character
		const string* findHeader(const vector<const string*>* headers_)
		{
			return ASBeautifier::findHeader(currentLine, charNum, headers_);
		}

		// call ASBase::findOperator for the current character
		const string* findOperator(const vector<const string*>* headers_)
		{
			return ASBeautifier::findOperator(currentLine, charNum, headers_);
		}

	private:  // variables
		int formatterFileType;
		vector<const string*>* headers;
		vector<const string*>* nonParenHeaders;
		vector<const string*>* preDefinitionHeaders;
		vector<const string*>* preCommandHeaders;
		vector<const string*>* operators;
		vector<const string*>* assignmentOperators;
		vector<const string*>* castOperators;
		vector<const pair<const string, const string>* >* indentableMacros; // for ASEnhancer

		ASSourceIterator* sourceIterator;
		ASEnhancer* enhancer;

		vector<const string*>* preBracketHeaderStack;
		vector<BracketType>* bracketTypeStack;
		vector<int>* parenStack;
		vector<bool>* structStack;
		vector<bool>* questionMarkStack;

		string currentLine;
		string formattedLine;
		string readyFormattedLine;
		string verbatimDelimiter;
		const string* currentHeader;
		const string* previousOperator;    // used ONLY by pad-oper
		char currentChar;
		char previousChar;
		char previousNonWSChar;
		char previousCommandChar;
		char quoteChar;
		streamoff preprocBlockEnd;
		int  charNum;
		int  horstmannIndentChars;
		int  nextLineSpacePadNum;
		int  preprocBracketTypeStackSize;
		int  spacePadNum;
		int  tabIncrementIn;
		int  templateDepth;
		int  squareBracketCount;
		size_t checksumIn;
		size_t checksumOut;
		size_t currentLineFirstBracketNum;  // first bracket location on currentLine
		size_t formattedLineCommentNum;     // comment location on formattedLine
		size_t leadingSpaces;
		size_t maxCodeLength;

		// possible split points
		size_t maxSemi;         // probably a 'for' statement
		size_t maxAndOr;        // probably an 'if' statement
		size_t maxComma;
		size_t maxParen;
		size_t maxWhiteSpace;
		size_t maxSemiPending;
		size_t maxAndOrPending;
		size_t maxCommaPending;
		size_t maxParenPending;
		size_t maxWhiteSpacePending;

		size_t previousReadyFormattedLineLength;
		FormatStyle formattingStyle;
		BracketMode bracketFormatMode;
		BracketType previousBracketType;
		PointerAlign pointerAlignment;
		ReferenceAlign referenceAlignment;
		ObjCColonPad objCColonPadMode;
		LineEndFormat lineEnd;
		bool isVirgin;
		bool shouldPadOperators;
		bool shouldPadParensOutside;
		bool shouldPadFirstParen;
		bool shouldPadParensInside;
		bool shouldPadHeader;
		bool shouldStripCommentPrefix;
		bool shouldUnPadParens;
		bool shouldConvertTabs;
		bool shouldIndentCol1Comments;
		bool shouldIndentPreprocBlock;
		bool shouldCloseTemplates;
		bool shouldAttachExternC;
		bool shouldAttachNamespace;
		bool shouldAttachClass;
		bool shouldAttachInline;
		bool isInLineComment;
		bool isInComment;
		bool isInCommentStartLine;
		bool noTrimCommentContinuation;
		bool isInPreprocessor;
		bool isInPreprocessorBeautify;
		bool isInTemplate;
		bool doesLineStartComment;
		bool lineEndsInCommentOnly;
		bool lineIsCommentOnly;
		bool lineIsLineCommentOnly;
		bool lineIsEmpty;
		bool isImmediatelyPostCommentOnly;
		bool isImmediatelyPostEmptyLine;
		bool isInClassInitializer;
		bool isInQuote;
		bool isInVerbatimQuote;
		bool haveLineContinuationChar;
		bool isInQuoteContinuation;
		bool isHeaderInMultiStatementLine;
		bool isSpecialChar;
		bool isNonParenHeader;
		bool foundQuestionMark;
		bool foundPreDefinitionHeader;
		bool foundNamespaceHeader;
		bool foundClassHeader;
		bool foundStructHeader;
		bool foundInterfaceHeader;
		bool foundPreCommandHeader;
		bool foundPreCommandMacro;
		bool foundCastOperator;
		bool isInLineBreak;
		bool endOfAsmReached;
		bool endOfCodeReached;
		bool lineCommentNoIndent;
		bool isFormattingModeOff;
		bool isInEnum;
		bool isInExecSQL;
		bool isInAsm;
		bool isInAsmOneLine;
		bool isInAsmBlock;
		bool isLineReady;
		bool elseHeaderFollowsComments;
		bool caseHeaderFollowsComments;
		bool isPreviousBracketBlockRelated;
		bool isInPotentialCalculation;
		bool isCharImmediatelyPostComment;
		bool isPreviousCharPostComment;
		bool isCharImmediatelyPostLineComment;
		bool isCharImmediatelyPostOpenBlock;
		bool isCharImmediatelyPostCloseBlock;
		bool isCharImmediatelyPostTemplate;
		bool isCharImmediatelyPostReturn;
		bool isCharImmediatelyPostThrow;
		bool isCharImmediatelyPostOperator;
		bool isCharImmediatelyPostPointerOrReference;
		bool isInObjCMethodDefinition;
		bool isInObjCInterface;
		bool isInObjCSelector;
		bool breakCurrentOneLineBlock;
		bool shouldRemoveNextClosingBracket;
		bool isInHorstmannRunIn;
		bool currentLineBeginsWithBracket;
		bool attachClosingBracketMode;
		bool shouldBreakOneLineBlocks;
		bool shouldReparseCurrentChar;
		bool shouldBreakOneLineStatements;
		bool shouldBreakClosingHeaderBrackets;
		bool shouldBreakElseIfs;
		bool shouldBreakLineAfterLogical;
		bool shouldAddBrackets;
		bool shouldAddOneLineBrackets;
		bool shouldRemoveBrackets;
		bool shouldPadMethodColon;
		bool shouldPadMethodPrefix;
		bool shouldUnPadMethodPrefix;
		bool shouldDeleteEmptyLines;
		bool needHeaderOpeningBracket;
		bool shouldBreakLineAtNextChar;
		bool shouldKeepLineUnbroken;
		bool passedSemicolon;
		bool passedColon;
		bool isImmediatelyPostNonInStmt;
		bool isCharImmediatelyPostNonInStmt;
		bool isImmediatelyPostComment;
		bool isImmediatelyPostLineComment;
		bool isImmediatelyPostEmptyBlock;
		bool isImmediatelyPostPreprocessor;
		bool isImmediatelyPostReturn;
		bool isImmediatelyPostThrow;
		bool isImmediatelyPostOperator;
		bool isImmediatelyPostTemplate;
		bool isImmediatelyPostPointerOrReference;
		bool shouldBreakBlocks;
		bool shouldBreakClosingHeaderBlocks;
		bool isPrependPostBlockEmptyLineRequested;
		bool isAppendPostBlockEmptyLineRequested;
		bool isIndentableProprocessor;
		bool isIndentableProprocessorBlock;
		bool prependEmptyLine;
		bool appendOpeningBracket;
		bool foundClosingHeader;
		bool isInHeader;
		bool isImmediatelyPostHeader;
		bool isInCase;
		bool isFirstPreprocConditional;
		bool processedFirstConditional;
		bool isJavaStaticConstructor;

};

}   // end of astyle namespace
