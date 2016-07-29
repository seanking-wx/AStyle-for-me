#include "ASOptions.h"

namespace AStyle
{

//-----------------------------------------------------------------------------
// ASOptions class
// used by both console and library builds
//-----------------------------------------------------------------------------
bool ASOptions::ParseOptions(istream &in, const string &errorInfo)
{
	vector<string> fileOptionsVector;   // options from the options file
	ImportOptions(in, fileOptionsVector);
	return ParseOptions(fileOptionsVector, errorInfo);
}

/**
 * parse the options vector
 * optionsVector can be either a fileOptionsVector (options file) or an optionsVector (command line)
 *
 * @return        true if no errors, false if errors
 */
bool ASOptions::ParseOptions(vector<string> &optionsVector, const string &errorInfo)
{
	vector<string>::iterator option;
	string arg;
	m_Error.clear();
	for(option = optionsVector.begin(); option != optionsVector.end(); ++option)
	{
		arg = *option;
		if(arg[0] != '-')
			ParseOption(arg, errorInfo);
		else if(arg[1] == '-')
			ParseOption(arg.substr(2), errorInfo);
		else
		{
			size_t i;
			string subArg;
			for(i = 1; i < arg.length(); ++i)
			{
				if(i > 1
				   && isalpha((unsigned char)arg[i])
				   && arg[i - 1] != 'x')
				{
					// parse the previous option in subArg
					ParseOption(subArg, errorInfo);
					subArg.clear();
				}
				// append the current option to subArg
				subArg.append(1, arg[i]);
			}
			// parse the last option
			ParseOption(subArg, errorInfo);
		}
	}
	if(m_Error.str().length() > 0)
		return false;
	return true;
}

void ASOptions::ParseOption(const string &arg, const string &errorInfo)
{
	if(IsOption3(arg, "style=allman", "style=bsd", "style=break"))
		m_Formatter.setFormattingStyle(STYLE_ALLMAN);
	else if(IsOption2(arg, "style=java", "style=attach"))
		m_Formatter.setFormattingStyle(STYLE_JAVA);
	else if(IsOption3(arg, "style=k&r", "style=kr", "style=k/r"))
		m_Formatter.setFormattingStyle(STYLE_KR);
	else if(IsOption(arg, "style=stroustrup"))
		m_Formatter.setFormattingStyle(STYLE_STROUSTRUP);
	else if(IsOption(arg, "style=whitesmith"))
		m_Formatter.setFormattingStyle(STYLE_WHITESMITH);
	else if(IsOption(arg, "style=vtk"))
		m_Formatter.setFormattingStyle(STYLE_VTK);
	else if(IsOption(arg, "style=banner"))
		m_Formatter.setFormattingStyle(STYLE_BANNER);
	else if(IsOption(arg, "style=gnu"))
		m_Formatter.setFormattingStyle(STYLE_GNU);
	else if(IsOption2(arg, "style=linux", "style=knf"))
		m_Formatter.setFormattingStyle(STYLE_LINUX);
	else if(IsOption(arg, "style=horstmann"))
		m_Formatter.setFormattingStyle(STYLE_HORSTMANN);
	else if(IsOption2(arg, "style=1tbs", "style=otbs"))
		m_Formatter.setFormattingStyle(STYLE_1TBS);
	else if(IsOption(arg, "style=google"))
		m_Formatter.setFormattingStyle(STYLE_GOOGLE);
	else if(IsOption(arg, "style=pico"))
		m_Formatter.setFormattingStyle(STYLE_PICO);
	else if(IsOption2(arg, "style=lisp", "style=python"))
		m_Formatter.setFormattingStyle(STYLE_LISP);
	else if(IsParamOption(arg, "A"))
	{
		int style = 0;
		string styleParam = GetParam(arg, "A");
		if(styleParam.length() > 0)
			style = atoi(styleParam.c_str());
		if(style == 1)
			m_Formatter.setFormattingStyle(STYLE_ALLMAN);
		else if(style == 2)
			m_Formatter.setFormattingStyle(STYLE_JAVA);
		else if(style == 3)
			m_Formatter.setFormattingStyle(STYLE_KR);
		else if(style == 4)
			m_Formatter.setFormattingStyle(STYLE_STROUSTRUP);
		else if(style == 5)
			m_Formatter.setFormattingStyle(STYLE_WHITESMITH);
		else if(style == 6)
			m_Formatter.setFormattingStyle(STYLE_BANNER);
		else if(style == 7)
			m_Formatter.setFormattingStyle(STYLE_GNU);
		else if(style == 8)
			m_Formatter.setFormattingStyle(STYLE_LINUX);
		else if(style == 9)
			m_Formatter.setFormattingStyle(STYLE_HORSTMANN);
		else if(style == 10)
			m_Formatter.setFormattingStyle(STYLE_1TBS);
		else if(style == 11)
			m_Formatter.setFormattingStyle(STYLE_PICO);
		else if(style == 12)
			m_Formatter.setFormattingStyle(STYLE_LISP);
		else if(style == 14)
			m_Formatter.setFormattingStyle(STYLE_GOOGLE);
		else if(style == 15)
			m_Formatter.setFormattingStyle(STYLE_VTK);
		else
			IsOptionError(arg, errorInfo);
	}
	// must check for mode=cs before mode=c !!!
	else if(IsOption(arg, "mode=cs"))
	{
		m_Formatter.setSharpStyle();
		m_Formatter.setModeManuallySet(true);
	}
	else if(IsOption(arg, "mode=c"))
	{
		m_Formatter.setCStyle();
		m_Formatter.setModeManuallySet(true);
	}
	else if(IsOption(arg, "mode=java"))
	{
		m_Formatter.setJavaStyle();
		m_Formatter.setModeManuallySet(true);
	}
	else if(IsParamOption2(arg, "t", "indent=tab="))
	{
		int spaceNum = 4;
		string spaceNumParam = GetParam2(arg, "t", "indent=tab=");
		if(spaceNumParam.length() > 0)
			spaceNum = atoi(spaceNumParam.c_str());
		if(spaceNum < 2 || spaceNum > 20)
			IsOptionError(arg, errorInfo);
		else
			m_Formatter.setTabIndentation(spaceNum, false);
	}
	else if(IsOption(arg, "indent=tab"))
		m_Formatter.setTabIndentation(4);
	else if(IsParamOption2(arg, "T", "indent=force-tab="))
	{
		int spaceNum = 4;
		string spaceNumParam = GetParam2(arg, "T", "indent=force-tab=");
		if(spaceNumParam.length() > 0)
			spaceNum = atoi(spaceNumParam.c_str());
		if(spaceNum < 2 || spaceNum > 20)
			IsOptionError(arg, errorInfo);
		else
			m_Formatter.setTabIndentation(spaceNum, true);
	}
	else if(IsOption(arg, "indent=force-tab"))
		m_Formatter.setTabIndentation(4, true);
	else if(IsParamOption2(arg, "xT", "indent=force-tab-x="))
	{
		int tabNum = 8;
		string tabNumParam = GetParam2(arg, "xT", "indent=force-tab-x=");
		if(tabNumParam.length() > 0)
			tabNum = atoi(tabNumParam.c_str());
		if(tabNum < 2 || tabNum > 20)
			IsOptionError(arg, errorInfo);
		else
			m_Formatter.setForceTabXIndentation(tabNum);
	}
	else if(IsOption(arg, "indent=force-tab-x"))
		m_Formatter.setForceTabXIndentation(8);
	else if(IsParamOption2(arg, "s", "indent=spaces="))
	{
		int spaceNum = 4;
		string spaceNumParam = GetParam2(arg, "s", "indent=spaces=");
		if(spaceNumParam.length() > 0)
			spaceNum = atoi(spaceNumParam.c_str());
		if(spaceNum < 2 || spaceNum > 20)
			IsOptionError(arg, errorInfo);
		else
			m_Formatter.setSpaceIndentation(spaceNum);
	}
	else if(IsOption(arg, "indent=spaces"))
		m_Formatter.setSpaceIndentation(4);
	else if(IsParamOption2(arg, "m", "min-conditional-indent="))
	{
		int minIndent = MINCOND_TWO;
		string minIndentParam = GetParam2(arg, "m", "min-conditional-indent=");
		if(minIndentParam.length() > 0)
			minIndent = atoi(minIndentParam.c_str());
		if(minIndent >= MINCOND_END)
			IsOptionError(arg, errorInfo);
		else
			m_Formatter.setMinConditionalIndentOption(minIndent);
	}
	else if(IsParamOption2(arg, "M", "max-instatement-indent="))
	{
		int maxIndent = 40;
		string maxIndentParam = GetParam2(arg, "M", "max-instatement-indent=");
		if(maxIndentParam.length() > 0)
			maxIndent = atoi(maxIndentParam.c_str());
		if(maxIndent < 40)
			IsOptionError(arg, errorInfo);
		else if(maxIndent > 120)
			IsOptionError(arg, errorInfo);
		else
			m_Formatter.setMaxInStatementIndentLength(maxIndent);
	}
	else if(IsOption2(arg, "N", "indent-namespaces"))
		m_Formatter.setNamespaceIndent(true);
	else if(IsOption2(arg, "C", "indent-classes"))
		m_Formatter.setClassIndent(true);
	else if(IsOption2(arg, "xG", "indent-modifiers"))
		m_Formatter.setModifierIndent(true);
	else if(IsOption2(arg, "S", "indent-switches"))
		m_Formatter.setSwitchIndent(true);
	else if(IsOption2(arg, "K", "indent-cases"))
		m_Formatter.setCaseIndent(true);
	else if(IsOption2(arg, "L", "indent-labels"))
		m_Formatter.setLabelIndent(true);
	else if(IsOption2(arg, "xW", "indent-preproc-block"))
		m_Formatter.setPreprocBlockIndent(true);
	else if(IsOption2(arg, "w", "indent-preproc-define"))
		m_Formatter.setPreprocDefineIndent(true);
	else if(IsOption2(arg, "xw", "indent-preproc-cond"))
		m_Formatter.setPreprocConditionalIndent(true);
	else if(IsOption2(arg, "y", "break-closing-brackets"))
		m_Formatter.setBreakClosingHeaderBracketsMode(true);
	else if(IsOption2(arg, "O", "keep-one-line-blocks"))
		m_Formatter.setBreakOneLineBlocksMode(false);
	else if(IsOption2(arg, "o", "keep-one-line-statements"))
		m_Formatter.setSingleStatementsMode(false);
	else if(IsOption2(arg, "P", "pad-paren"))
	{
		m_Formatter.setParensOutsidePaddingMode(true);
		m_Formatter.setParensInsidePaddingMode(true);
	}
	else if(IsOption2(arg, "d", "pad-paren-out"))
		m_Formatter.setParensOutsidePaddingMode(true);
	else if(IsOption2(arg, "xd", "pad-first-paren-out"))
		m_Formatter.setParensFirstPaddingMode(true);
	else if(IsOption2(arg, "D", "pad-paren-in"))
		m_Formatter.setParensInsidePaddingMode(true);
	else if(IsOption2(arg, "H", "pad-header"))
		m_Formatter.setParensHeaderPaddingMode(true);
	else if(IsOption2(arg, "U", "unpad-paren"))
		m_Formatter.setParensUnPaddingMode(true);
	else if(IsOption2(arg, "p", "pad-oper"))
		m_Formatter.setOperatorPaddingMode(true);
	else if(IsOption2(arg, "xe", "delete-empty-lines"))
		m_Formatter.setDeleteEmptyLinesMode(true);
	else if(IsOption2(arg, "E", "fill-empty-lines"))
		m_Formatter.setEmptyLineFill(true);
	else if(IsOption2(arg, "c", "convert-tabs"))
		m_Formatter.setTabSpaceConversionMode(true);
	else if(IsOption2(arg, "xy", "close-templates"))
		m_Formatter.setCloseTemplatesMode(true);
	else if(IsOption2(arg, "F", "break-blocks=all"))
	{
		m_Formatter.setBreakBlocksMode(true);
		m_Formatter.setBreakClosingHeaderBlocksMode(true);
	}
	else if(IsOption2(arg, "f", "break-blocks"))
		m_Formatter.setBreakBlocksMode(true);
	else if(IsOption2(arg, "e", "break-elseifs"))
		m_Formatter.setBreakElseIfsMode(true);
	else if(IsOption2(arg, "j", "add-brackets"))
		m_Formatter.setAddBracketsMode(true);
	else if(IsOption2(arg, "J", "add-one-line-brackets"))
		m_Formatter.setAddOneLineBracketsMode(true);
	else if(IsOption2(arg, "xj", "remove-brackets"))
		m_Formatter.setRemoveBracketsMode(true);
	else if(IsOption2(arg, "Y", "indent-col1-comments"))
		m_Formatter.setIndentCol1CommentsMode(true);
	else if(IsOption(arg, "align-pointer=type"))
		m_Formatter.setPointerAlignment(PTR_ALIGN_TYPE);
	else if(IsOption(arg, "align-pointer=middle"))
		m_Formatter.setPointerAlignment(PTR_ALIGN_MIDDLE);
	else if(IsOption(arg, "align-pointer=name"))
		m_Formatter.setPointerAlignment(PTR_ALIGN_NAME);
	else if(IsParamOption(arg, "k"))
	{
		int align = 0;
		string styleParam = GetParam(arg, "k");
		if(styleParam.length() > 0)
			align = atoi(styleParam.c_str());
		if(align < 1 || align > 3)
			IsOptionError(arg, errorInfo);
		else if(align == 1)
			m_Formatter.setPointerAlignment(PTR_ALIGN_TYPE);
		else if(align == 2)
			m_Formatter.setPointerAlignment(PTR_ALIGN_MIDDLE);
		else if(align == 3)
			m_Formatter.setPointerAlignment(PTR_ALIGN_NAME);
	}
	else if(IsOption(arg, "align-reference=none"))
		m_Formatter.setReferenceAlignment(REF_ALIGN_NONE);
	else if(IsOption(arg, "align-reference=type"))
		m_Formatter.setReferenceAlignment(REF_ALIGN_TYPE);
	else if(IsOption(arg, "align-reference=middle"))
		m_Formatter.setReferenceAlignment(REF_ALIGN_MIDDLE);
	else if(IsOption(arg, "align-reference=name"))
		m_Formatter.setReferenceAlignment(REF_ALIGN_NAME);
	else if(IsParamOption(arg, "W"))
	{
		int align = 0;
		string styleParam = GetParam(arg, "W");
		if(styleParam.length() > 0)
			align = atoi(styleParam.c_str());
		if(align < 0 || align > 3)
			IsOptionError(arg, errorInfo);
		else if(align == 0)
			m_Formatter.setReferenceAlignment(REF_ALIGN_NONE);
		else if(align == 1)
			m_Formatter.setReferenceAlignment(REF_ALIGN_TYPE);
		else if(align == 2)
			m_Formatter.setReferenceAlignment(REF_ALIGN_MIDDLE);
		else if(align == 3)
			m_Formatter.setReferenceAlignment(REF_ALIGN_NAME);
	}
	else if(IsParamOption(arg, "max-code-length="))
	{
		int maxLength = 50;
		string maxLengthParam = GetParam(arg, "max-code-length=");
		if(maxLengthParam.length() > 0)
			maxLength = atoi(maxLengthParam.c_str());
		if(maxLength < 50)
			IsOptionError(arg, errorInfo);
		else if(maxLength > 200)
			IsOptionError(arg, errorInfo);
		else
			m_Formatter.setMaxCodeLength(maxLength);
	}
	else if(IsParamOption(arg, "xC"))
	{
		int maxLength = 50;
		string maxLengthParam = GetParam(arg, "xC");
		if(maxLengthParam.length() > 0)
			maxLength = atoi(maxLengthParam.c_str());
		if(maxLength > 200)
			IsOptionError(arg, errorInfo);
		else
			m_Formatter.setMaxCodeLength(maxLength);
	}
	else if(IsOption2(arg, "xL", "break-after-logical"))
		m_Formatter.setBreakAfterMode(true);
	else if(IsOption2(arg, "xc", "attach-classes"))
		m_Formatter.setAttachClass(true);
	else if(IsOption2(arg, "xk", "attach-extern-c"))
		m_Formatter.setAttachExternC(true);
	else if(IsOption2(arg, "xn", "attach-namespaces"))
		m_Formatter.setAttachNamespace(true);
	else if(IsOption2(arg, "xl", "attach-inlines"))
		m_Formatter.setAttachInline(true);
	else if(IsOption2(arg, "xp", "remove-comment-prefix"))
		m_Formatter.setStripCommentPrefix(true);
	// Objective-C options
	else if(IsOption2(arg, "xM", "align-method-colon"))
		m_Formatter.setAlignMethodColon(true);
	else if(IsOption2(arg, "xQ", "pad-method-prefix"))
		m_Formatter.setMethodPrefixPaddingMode(true);
	else if(IsOption2(arg, "xR", "unpad-method-prefix"))
		m_Formatter.setMethodPrefixUnPaddingMode(true);
	else if(IsOption2(arg, "xP0", "pad-method-colon=none"))
		m_Formatter.setObjCColonPaddingMode(COLON_PAD_NONE);
	else if(IsOption2(arg, "xP1", "pad-method-colon=all"))
		m_Formatter.setObjCColonPaddingMode(COLON_PAD_ALL);
	else if(IsOption2(arg, "xP2", "pad-method-colon=after"))
		m_Formatter.setObjCColonPaddingMode(COLON_PAD_AFTER);
	else if(IsOption2(arg, "xP3", "pad-method-colon=before"))
		m_Formatter.setObjCColonPaddingMode(COLON_PAD_BEFORE);
	// depreciated options ////////////////////////////////////////////////////////////////////////////////////////////
	else if(IsOption(arg, "indent-preprocessor"))    // depreciated release 2.04
		m_Formatter.setPreprocDefineIndent(true);
	else if(IsOption(arg, "style=ansi"))                 // depreciated release 2.05
		m_Formatter.setFormattingStyle(STYLE_ALLMAN);
	//  NOTE: Removed in release 2.04.
	//  else if ( IsOption2(arg, "b", "brackets=break") )
	//  {
	//      m_Formatter.setBracketFormatMode(BREAK_MODE);
	//  }
	//  else if ( IsOption2(arg, "a", "brackets=attach") )
	//  {
	//      m_Formatter.setBracketFormatMode(ATTACH_MODE);
	//  }
	//  else if ( IsOption2(arg, "l", "brackets=linux") )
	//  {
	//      m_Formatter.setBracketFormatMode(LINUX_MODE);
	//  }
	//  else if ( IsOption2(arg, "u", "brackets=stroustrup") )
	//  {
	//      m_Formatter.setBracketFormatMode(STROUSTRUP_MODE);
	//  }
	//  else if ( IsOption2(arg, "g", "brackets=run-in") )
	//  {
	//      m_Formatter.setBracketFormatMode(RUN_IN_MODE);
	//  }
	// end depreciated options ////////////////////////////////////////////////////////////////////////////////////////
#ifdef ASTYLE_LIB
	// End of options used by GUI /////////////////////////////////////////////////////////////////////////////////////
	else
		IsOptionError(arg, errorInfo);
#else
	else if(IsOption(arg, "lineend=windows"))
		m_Formatter.setLineEndFormat(LINEEND_WINDOWS);
	else if(IsOption(arg, "lineend=linux"))
		m_Formatter.setLineEndFormat(LINEEND_LINUX);
	else if(IsOption(arg, "lineend=macold"))
		m_Formatter.setLineEndFormat(LINEEND_MACOLD);
	else if(IsParamOption(arg, "z"))
	{
		int lineendType = 0;
		string lineendParam = GetParam(arg, "z");
		if(lineendParam.length() > 0)
			lineendType = atoi(lineendParam.c_str());
		if(lineendType < 1 || lineendType > 3)
			IsOptionError(arg, errorInfo);
		else if(lineendType == 1)
			m_Formatter.setLineEndFormat(LINEEND_WINDOWS);
		else if(lineendType == 2)
			m_Formatter.setLineEndFormat(LINEEND_LINUX);
		else if(lineendType == 3)
			m_Formatter.setLineEndFormat(LINEEND_MACOLD);
	}
	// Options used by only console ///////////////////////////////////////////////////////////////////////////////////
	else if(IsOption2(arg, "n", "suffix=none"))
		m_Console.setNoBackup(true);
	else if(IsParamOption(arg, "suffix="))
	{
		string suffixParam = GetParam(arg, "suffix=");
		if(suffixParam.length() > 0)
			m_Console.setOrigSuffix(suffixParam);
	}
	else if(IsParamOption(arg, "exclude="))
	{
		string suffixParam = GetParam(arg, "exclude=");
		if(suffixParam.length() > 0)
			m_Console.SetExcludeVector(suffixParam,false);
	}
	else if(IsParamOption(arg, "exclude-name="))
	{
		string suffixParam = GetParam(arg, "exclude-name=");
		if(suffixParam.length() > 0)
			m_Console.SetExcludeVector(suffixParam,true);
	}
	else if(IsOption3(arg, "r", "R", "recursive"))
		m_Console.setIsRecursive(true);
	else if(IsOption(arg, "dry-run"))
		m_Console.setIsDryRun(true);
	else if(IsOption2(arg, "Z", "preserve-date"))
		m_Console.setPreserveDate(true);
	else if(IsOption2(arg, "v", "verbose"))
		m_Console.SetIsVerbose(true);
	else if(IsOption2(arg, "Q", "formatted"))
		m_Console.setIsFormattedOnly(true);
	else if(IsOption2(arg, "q", "quiet"))
		m_Console.setIsQuiet(true);
	else if(IsOption2(arg, "i", "ignore-exclude-errors"))
		m_Console.SetIgnoreExcludeErrors(true);
	else if(IsOption2(arg, "xi", "ignore-exclude-errors-x"))
		m_Console.SetIgnoreExcludeErrorsAndDisplay(true);
	else if(IsOption(arg, "ignore-empty-errors"))
		m_Console.SetIgnoreEmptyErrors(true);
	else if(IsOption(arg, "ignore-empty-errors-x"))
		m_Console.SetIgnoreEmptyErrorsAndDisplay(true);
	else if(IsOption2(arg, "X", "errors-to-stdout"))
		m_Console.SetErrorOut(&cout);
	else
		IsOptionError(arg, errorInfo);
#endif
}   // End of parseOption function

// Parse options from the options file.
void ASOptions::ImportOptions(istream &in, vector<string> &optionsVector)
{
	char ch;
	bool isInQuote = false;
	char quoteChar = ' ';
	string currentToken;
	while(in)
	{
		currentToken = "";
		do
		{
			in.get(ch);
			if(in.eof())
				break;
			// treat '#' as line comments
			if(ch == '#')
				while(in)
				{
					in.get(ch);
					if(ch == '\n' || ch == '\r')
						break;
				}
			// break options on new-lines, tabs, commas, or spaces
			// remove quotes from output
			if(in.eof() || ch == '\n' || ch == '\r' || ch == '\t' || ch == ',')
				break;
			if(ch == ' ' && !isInQuote)
				break;
			if(ch == quoteChar && isInQuote)
				break;
			if(ch == '"' || ch == '\'')
			{
				isInQuote = true;
				quoteChar = ch;
				continue;
			}
			currentToken.append(1, ch);
		}
		while(in);
		if(currentToken.length() != 0)
			optionsVector.push_back(currentToken);
		isInQuote = false;
	}
}

}   // end of astyle namespace
