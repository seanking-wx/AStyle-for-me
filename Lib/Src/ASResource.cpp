#include "ASResource.h"
#include "AStyle_FileType.h"
#include <algorithm>

namespace AStyle
{

/**
 * Build the vector of assignment operators.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp
 *
 * @param assignmentOperators   a reference to the vector to be built.
 */
void ASResource::buildAssignmentOperators(vector<const string*>* assignmentOperators)
{
	assignmentOperators->push_back(&AS_ASSIGN);
	assignmentOperators->push_back(&AS_PLUS_ASSIGN);
	assignmentOperators->push_back(&AS_MINUS_ASSIGN);
	assignmentOperators->push_back(&AS_MULT_ASSIGN);
	assignmentOperators->push_back(&AS_DIV_ASSIGN);
	assignmentOperators->push_back(&AS_MOD_ASSIGN);
	assignmentOperators->push_back(&AS_OR_ASSIGN);
	assignmentOperators->push_back(&AS_AND_ASSIGN);
	assignmentOperators->push_back(&AS_XOR_ASSIGN);
	// Java
	assignmentOperators->push_back(&AS_GR_GR_GR_ASSIGN);
	assignmentOperators->push_back(&AS_GR_GR_ASSIGN);
	assignmentOperators->push_back(&AS_LS_LS_ASSIGN);
	// Unknown
	assignmentOperators->push_back(&AS_LS_LS_LS_ASSIGN);
	sort(assignmentOperators->begin(), assignmentOperators->end(), sortOnLength);
}

/**
 * Build the vector of C++ cast operators.
 * Used by ONLY ASFormatter.cpp
 *
 * @param castOperators     a reference to the vector to be built.
 */
void ASResource::buildCastOperators(vector<const string*>* castOperators)
{
	castOperators->push_back(&AS_CONST_CAST);
	castOperators->push_back(&AS_DYNAMIC_CAST);
	castOperators->push_back(&AS_REINTERPRET_CAST);
	castOperators->push_back(&AS_STATIC_CAST);
}

/**
 * Build the vector of header words.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp
 *
 * @param headers       a reference to the vector to be built.
 */
void ASResource::buildHeaders(vector<const string*>* headers, int fileType, bool beautifier)
{
	headers->push_back(&AS_IF);
	headers->push_back(&AS_ELSE);
	headers->push_back(&AS_FOR);
	headers->push_back(&AS_WHILE);
	headers->push_back(&AS_DO);
	headers->push_back(&AS_SWITCH);
	headers->push_back(&AS_CASE);
	headers->push_back(&AS_DEFAULT);
	headers->push_back(&AS_TRY);
	headers->push_back(&AS_CATCH);
	headers->push_back(&AS_QFOREACH);       // QT
	headers->push_back(&AS_QFOREVER);       // QT
	headers->push_back(&AS_FOREACH);        // QT & C#
	headers->push_back(&AS_FOREVER);        // Qt & Boost
	if(fileType == C_TYPE)
	{
		headers->push_back(&_AS_TRY);       // __try
		headers->push_back(&_AS_FINALLY);   // __finally
		headers->push_back(&_AS_EXCEPT);    // __except
	}
	if(fileType == JAVA_TYPE)
	{
		headers->push_back(&AS_FINALLY);
		headers->push_back(&AS_SYNCHRONIZED);
	}
	if(fileType == SHARP_TYPE)
	{
		headers->push_back(&AS_FINALLY);
		headers->push_back(&AS_LOCK);
		headers->push_back(&AS_FIXED);
		headers->push_back(&AS_GET);
		headers->push_back(&AS_SET);
		headers->push_back(&AS_ADD);
		headers->push_back(&AS_REMOVE);
	}
	if(beautifier)
	{
		if(fileType == C_TYPE)
			headers->push_back(&AS_TEMPLATE);
		if(fileType == JAVA_TYPE)
		{
			headers->push_back(&AS_STATIC);         // for static constructor
		}
	}
	sort(headers->begin(), headers->end(), sortOnName);
}

/**
 * Build the vector of indentable headers.
 * Used by ONLY ASBeautifier.cpp
 *
 * @param indentableHeaders     a reference to the vector to be built.
 */
void ASResource::buildIndentableHeaders(vector<const string*>* indentableHeaders)
{
	indentableHeaders->push_back(&AS_RETURN);
	sort(indentableHeaders->begin(), indentableHeaders->end(), sortOnName);
}

/**
* Build the vector of indentable macros pairs.
* Initialized by ASFormatter, used by ONLY ASEnhancer.cpp
*
* @param indentableMacros       a reference to the vector to be built.
*/
void ASResource::buildIndentableMacros(vector<const pair<const string, const string>* >* indentableMacros)
{
	// the pairs must be retained in memory
	static const struct pair<const string, const string> macros[] =
	{
		// wxWidgets
		make_pair("BEGIN_EVENT_TABLE", "END_EVENT_TABLE"),
		make_pair("wxBEGIN_EVENT_TABLE", "wxEND_EVENT_TABLE"),
		// MFC
		make_pair("BEGIN_DISPATCH_MAP", "END_DISPATCH_MAP"),
		make_pair("BEGIN_EVENT_MAP", "END_EVENT_MAP"),
		make_pair("BEGIN_MESSAGE_MAP", "END_MESSAGE_MAP"),
		make_pair("BEGIN_PROPPAGEIDS", "END_PROPPAGEIDS"),
	};
	size_t elements = sizeof(macros) / sizeof(macros[0]);
	for(size_t i = 0; i < elements; i++)
		indentableMacros->push_back(&macros[i]);
}

/**
 * Build the vector of non-assignment operators.
 * Used by ONLY ASBeautifier.cpp
 *
 * @param nonAssignmentOperators       a reference to the vector to be built.
 */
void ASResource::buildNonAssignmentOperators(vector<const string*>* nonAssignmentOperators)
{
	nonAssignmentOperators->push_back(&AS_EQUAL);
	nonAssignmentOperators->push_back(&AS_PLUS_PLUS);
	nonAssignmentOperators->push_back(&AS_MINUS_MINUS);
	nonAssignmentOperators->push_back(&AS_NOT_EQUAL);
	nonAssignmentOperators->push_back(&AS_GR_EQUAL);
	nonAssignmentOperators->push_back(&AS_GR_GR_GR);
	nonAssignmentOperators->push_back(&AS_GR_GR);
	nonAssignmentOperators->push_back(&AS_LS_EQUAL);
	nonAssignmentOperators->push_back(&AS_LS_LS_LS);
	nonAssignmentOperators->push_back(&AS_LS_LS);
	nonAssignmentOperators->push_back(&AS_ARROW);
	nonAssignmentOperators->push_back(&AS_AND);
	nonAssignmentOperators->push_back(&AS_OR);
	nonAssignmentOperators->push_back(&AS_LAMBDA);
	sort(nonAssignmentOperators->begin(), nonAssignmentOperators->end(), sortOnLength);
}

/**
 * Build the vector of header non-paren headers.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp.
 * NOTE: Non-paren headers should also be included in the headers vector.
 *
 * @param nonParenHeaders       a reference to the vector to be built.
 */
void ASResource::buildNonParenHeaders(vector<const string*>* nonParenHeaders, int fileType, bool beautifier)
{
	nonParenHeaders->push_back(&AS_ELSE);
	nonParenHeaders->push_back(&AS_DO);
	nonParenHeaders->push_back(&AS_TRY);
	nonParenHeaders->push_back(&AS_CATCH);      // can be paren or non-paren
	nonParenHeaders->push_back(&AS_CASE);       // can be paren or non-paren
	nonParenHeaders->push_back(&AS_DEFAULT);
	nonParenHeaders->push_back(&AS_QFOREVER);   // QT
	nonParenHeaders->push_back(&AS_FOREVER);    // Boost
	if(fileType == C_TYPE)
	{
		nonParenHeaders->push_back(&_AS_TRY);       // __try
		nonParenHeaders->push_back(&_AS_FINALLY);   // __finally
	}
	if(fileType == JAVA_TYPE)
		nonParenHeaders->push_back(&AS_FINALLY);
	if(fileType == SHARP_TYPE)
	{
		nonParenHeaders->push_back(&AS_FINALLY);
		nonParenHeaders->push_back(&AS_GET);
		nonParenHeaders->push_back(&AS_SET);
		nonParenHeaders->push_back(&AS_ADD);
		nonParenHeaders->push_back(&AS_REMOVE);
	}
	if(beautifier)
	{
		if(fileType == C_TYPE)
			nonParenHeaders->push_back(&AS_TEMPLATE);
		if(fileType == JAVA_TYPE)
			nonParenHeaders->push_back(&AS_STATIC);
	}
	sort(nonParenHeaders->begin(), nonParenHeaders->end(), sortOnName);
}

/**
 * Build the vector of operators.
 * Used by ONLY ASFormatter.cpp
 *
 * @param operators             a reference to the vector to be built.
 */
void ASResource::buildOperators(vector<const string*>* operators, int fileType)
{
	operators->push_back(&AS_PLUS_ASSIGN);
	operators->push_back(&AS_MINUS_ASSIGN);
	operators->push_back(&AS_MULT_ASSIGN);
	operators->push_back(&AS_DIV_ASSIGN);
	operators->push_back(&AS_MOD_ASSIGN);
	operators->push_back(&AS_OR_ASSIGN);
	operators->push_back(&AS_AND_ASSIGN);
	operators->push_back(&AS_XOR_ASSIGN);
	operators->push_back(&AS_EQUAL);
	operators->push_back(&AS_PLUS_PLUS);
	operators->push_back(&AS_MINUS_MINUS);
	operators->push_back(&AS_NOT_EQUAL);
	operators->push_back(&AS_GR_EQUAL);
	operators->push_back(&AS_GR_GR_GR_ASSIGN);
	operators->push_back(&AS_GR_GR_ASSIGN);
	operators->push_back(&AS_GR_GR_GR);
	operators->push_back(&AS_GR_GR);
	operators->push_back(&AS_LS_EQUAL);
	operators->push_back(&AS_LS_LS_LS_ASSIGN);
	operators->push_back(&AS_LS_LS_ASSIGN);
	operators->push_back(&AS_LS_LS_LS);
	operators->push_back(&AS_LS_LS);
	operators->push_back(&AS_QUESTION_QUESTION);
	operators->push_back(&AS_LAMBDA);
	operators->push_back(&AS_ARROW);
	operators->push_back(&AS_AND);
	operators->push_back(&AS_OR);
	operators->push_back(&AS_SCOPE_RESOLUTION);
	operators->push_back(&AS_PLUS);
	operators->push_back(&AS_MINUS);
	operators->push_back(&AS_MULT);
	operators->push_back(&AS_DIV);
	operators->push_back(&AS_MOD);
	operators->push_back(&AS_QUESTION);
	operators->push_back(&AS_COLON);
	operators->push_back(&AS_ASSIGN);
	operators->push_back(&AS_LS);
	operators->push_back(&AS_GR);
	operators->push_back(&AS_NOT);
	operators->push_back(&AS_BIT_OR);
	operators->push_back(&AS_BIT_AND);
	operators->push_back(&AS_BIT_NOT);
	operators->push_back(&AS_BIT_XOR);
	if(fileType == C_TYPE)
	{
		operators->push_back(&AS_GCC_MIN_ASSIGN);
		operators->push_back(&AS_GCC_MAX_ASSIGN);
	}
	sort(operators->begin(), operators->end(), sortOnLength);
}

/**
 * Build the vector of pre-block statements.
 * Used by ONLY ASBeautifier.cpp
 * NOTE: Cannot be both a header and a preBlockStatement.
 *
 * @param preBlockStatements        a reference to the vector to be built.
 */
void ASResource::buildPreBlockStatements(vector<const string*>* preBlockStatements, int fileType)
{
	preBlockStatements->push_back(&AS_CLASS);
	if(fileType == C_TYPE)
	{
		preBlockStatements->push_back(&AS_STRUCT);
		preBlockStatements->push_back(&AS_UNION);
		preBlockStatements->push_back(&AS_NAMESPACE);
	}
	if(fileType == JAVA_TYPE)
	{
		preBlockStatements->push_back(&AS_INTERFACE);
		preBlockStatements->push_back(&AS_THROWS);
	}
	if(fileType == SHARP_TYPE)
	{
		preBlockStatements->push_back(&AS_INTERFACE);
		preBlockStatements->push_back(&AS_NAMESPACE);
		preBlockStatements->push_back(&AS_WHERE);
		preBlockStatements->push_back(&AS_STRUCT);
	}
	sort(preBlockStatements->begin(), preBlockStatements->end(), sortOnName);
}

/**
 * Build the vector of pre-command headers.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp.
 * NOTE: Cannot be both a header and a preCommandHeader.
 *
 * A preCommandHeader is in a function definition between
 * the closing paren and the opening bracket.
 * e.g. in "void foo() const {}", "const" is a preCommandHeader.
 */
void ASResource::buildPreCommandHeaders(vector<const string*>* preCommandHeaders, int fileType)
{
	if(fileType == C_TYPE)
	{
		preCommandHeaders->push_back(&AS_CONST);
		preCommandHeaders->push_back(&AS_VOLATILE);
		preCommandHeaders->push_back(&AS_INTERRUPT);
		preCommandHeaders->push_back(&AS_NOEXCEPT);
		preCommandHeaders->push_back(&AS_OVERRIDE);
		preCommandHeaders->push_back(&AS_SEALED);           // Visual C only
		preCommandHeaders->push_back(&AS_AUTORELEASEPOOL);  // Obj-C only
	}
	if(fileType == JAVA_TYPE)
		preCommandHeaders->push_back(&AS_THROWS);
	if(fileType == SHARP_TYPE)
		preCommandHeaders->push_back(&AS_WHERE);
	sort(preCommandHeaders->begin(), preCommandHeaders->end(), sortOnName);
}

/**
 * Build the vector of pre-definition headers.
 * Used by ONLY ASFormatter.cpp
 * NOTE: Do NOT add 'enum' here. It is an array type bracket.
 * NOTE: Do NOT add 'extern' here. Do not want an extra indent.
 *
 * @param preDefinitionHeaders      a reference to the vector to be built.
 */
void ASResource::buildPreDefinitionHeaders(vector<const string*>* preDefinitionHeaders, int fileType)
{
	preDefinitionHeaders->push_back(&AS_CLASS);
	if(fileType == C_TYPE)
	{
		preDefinitionHeaders->push_back(&AS_STRUCT);
		preDefinitionHeaders->push_back(&AS_UNION);
		preDefinitionHeaders->push_back(&AS_NAMESPACE);
	}
	if(fileType == JAVA_TYPE)
		preDefinitionHeaders->push_back(&AS_INTERFACE);
	if(fileType == SHARP_TYPE)
	{
		preDefinitionHeaders->push_back(&AS_STRUCT);
		preDefinitionHeaders->push_back(&AS_INTERFACE);
		preDefinitionHeaders->push_back(&AS_NAMESPACE);
	}
	sort(preDefinitionHeaders->begin(), preDefinitionHeaders->end(), sortOnName);
}

}   // end of astyle namespace
