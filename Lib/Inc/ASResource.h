#pragma once
#include <string>
#include <vector>

namespace AStyle
{

using namespace std;

//-----------------------------------------------------------------------------
// Class ASResource
//-----------------------------------------------------------------------------

class ASResource
{
	public:
		ASResource() {}
		virtual ~ASResource() {}
		void buildAssignmentOperators(vector<const string*>* assignmentOperators);
		void buildCastOperators(vector<const string*>* castOperators);
		void buildHeaders(vector<const string*>* headers, int fileType, bool beautifier = false);
		void buildIndentableMacros(vector<const pair<const string, const string>* >* indentableMacros);
		void buildIndentableHeaders(vector<const string*>* indentableHeaders);
		void buildNonAssignmentOperators(vector<const string*>* nonAssignmentOperators);
		void buildNonParenHeaders(vector<const string*>* nonParenHeaders, int fileType, bool beautifier = false);
		void buildOperators(vector<const string*>* operators, int fileType);
		void buildPreBlockStatements(vector<const string*>* preBlockStatements, int fileType);
		void buildPreCommandHeaders(vector<const string*>* preCommandHeaders, int fileType);
		void buildPreDefinitionHeaders(vector<const string*>* preDefinitionHeaders, int fileType);

	public:
		static const string AS_IF, AS_ELSE;
		static const string AS_DO, AS_WHILE;
		static const string AS_FOR;
		static const string AS_SWITCH, AS_CASE, AS_DEFAULT;
		static const string AS_TRY, AS_CATCH, AS_THROW, AS_THROWS, AS_FINALLY;
		static const string _AS_TRY, _AS_FINALLY, _AS_EXCEPT;
		static const string AS_PUBLIC, AS_PROTECTED, AS_PRIVATE;
		static const string AS_CLASS, AS_STRUCT, AS_UNION, AS_INTERFACE, AS_NAMESPACE;
		static const string AS_END;
		static const string AS_SELECTOR;
		static const string AS_EXTERN, AS_ENUM;
		static const string AS_STATIC, AS_CONST, AS_SEALED, AS_OVERRIDE, AS_VOLATILE, AS_NEW;
		static const string AS_NOEXCEPT, AS_INTERRUPT, AS_AUTORELEASEPOOL;
		static const string AS_WHERE, AS_LET, AS_SYNCHRONIZED;
		static const string AS_OPERATOR, AS_TEMPLATE;
		static const string AS_OPEN_BRACKET, AS_CLOSE_BRACKET;
		static const string AS_OPEN_LINE_COMMENT, AS_OPEN_COMMENT, AS_CLOSE_COMMENT;
		static const string AS_BAR_DEFINE, AS_BAR_INCLUDE, AS_BAR_IF, AS_BAR_EL, AS_BAR_ENDIF;
		static const string AS_RETURN;
		static const string AS_CIN, AS_COUT, AS_CERR;
		static const string AS_ASSIGN, AS_PLUS_ASSIGN, AS_MINUS_ASSIGN, AS_MULT_ASSIGN;
		static const string AS_DIV_ASSIGN, AS_MOD_ASSIGN, AS_XOR_ASSIGN, AS_OR_ASSIGN, AS_AND_ASSIGN;
		static const string AS_GR_GR_ASSIGN, AS_LS_LS_ASSIGN, AS_GR_GR_GR_ASSIGN, AS_LS_LS_LS_ASSIGN;
		static const string AS_GCC_MIN_ASSIGN, AS_GCC_MAX_ASSIGN;
		static const string AS_EQUAL, AS_PLUS_PLUS, AS_MINUS_MINUS, AS_NOT_EQUAL, AS_GR_EQUAL, AS_GR_GR_GR, AS_GR_GR;
		static const string AS_LS_EQUAL, AS_LS_LS_LS, AS_LS_LS;
		static const string AS_QUESTION_QUESTION, AS_LAMBDA;
		static const string AS_ARROW, AS_AND, AS_OR;
		static const string AS_SCOPE_RESOLUTION;
		static const string AS_PLUS, AS_MINUS, AS_MULT, AS_DIV, AS_MOD, AS_GR, AS_LS;
		static const string AS_NOT, AS_BIT_XOR, AS_BIT_OR, AS_BIT_AND, AS_BIT_NOT;
		static const string AS_QUESTION, AS_COLON, AS_SEMICOLON, AS_COMMA;
		static const string AS_ASM, AS__ASM__, AS_MS_ASM, AS_MS__ASM;
		static const string AS_QFOREACH, AS_QFOREVER, AS_FOREVER;
		static const string AS_FOREACH, AS_LOCK, AS_UNSAFE, AS_FIXED;
		static const string AS_GET, AS_SET, AS_ADD, AS_REMOVE;
		static const string AS_DELEGATE, AS_UNCHECKED;
		static const string AS_CONST_CAST, AS_DYNAMIC_CAST, AS_REINTERPRET_CAST, AS_STATIC_CAST;
		static const string AS_NS_DURING, AS_NS_HANDLER;
};

// sort comparison functions for ASResource
bool sortOnLength(const string* a, const string* b);
bool sortOnName(const string* a, const string* b);

}   // end of astyle namespace
