#include "ASResource.h"

namespace AStyle
{

const string ASResource::AS_IF = string("if");
const string ASResource::AS_ELSE = string("else");
const string ASResource::AS_FOR = string("for");
const string ASResource::AS_DO = string("do");
const string ASResource::AS_WHILE = string("while");
const string ASResource::AS_SWITCH = string("switch");
const string ASResource::AS_CASE = string("case");
const string ASResource::AS_DEFAULT = string("default");
const string ASResource::AS_CLASS = string("class");
const string ASResource::AS_VOLATILE = string("volatile");
const string ASResource::AS_INTERRUPT = string("interrupt");
const string ASResource::AS_NOEXCEPT = string("noexcept");
const string ASResource::AS_AUTORELEASEPOOL = string("autoreleasepool");
const string ASResource::AS_STRUCT = string("struct");
const string ASResource::AS_UNION = string("union");
const string ASResource::AS_INTERFACE = string("interface");
const string ASResource::AS_NAMESPACE = string("namespace");
const string ASResource::AS_END = string("end");
const string ASResource::AS_SELECTOR = string("selector");
const string ASResource::AS_EXTERN = string("extern");
const string ASResource::AS_ENUM = string("enum");
const string ASResource::AS_PUBLIC = string("public");
const string ASResource::AS_PROTECTED = string("protected");
const string ASResource::AS_PRIVATE = string("private");
const string ASResource::AS_STATIC = string("static");
const string ASResource::AS_SYNCHRONIZED = string("synchronized");
const string ASResource::AS_OPERATOR = string("operator");
const string ASResource::AS_TEMPLATE = string("template");
const string ASResource::AS_TRY = string("try");
const string ASResource::AS_CATCH = string("catch");
const string ASResource::AS_THROW = string("throw");
const string ASResource::AS_FINALLY = string("finally");
const string ASResource::_AS_TRY = string("__try");
const string ASResource::_AS_FINALLY = string("__finally");
const string ASResource::_AS_EXCEPT = string("__except");
const string ASResource::AS_THROWS = string("throws");
const string ASResource::AS_CONST = string("const");
const string ASResource::AS_SEALED = string("sealed");
const string ASResource::AS_OVERRIDE = string("override");
const string ASResource::AS_WHERE = string("where");
const string ASResource::AS_LET = string("let");
const string ASResource::AS_NEW = string("new");

const string ASResource::AS_ASM = string("asm");
const string ASResource::AS__ASM__ = string("__asm__");
const string ASResource::AS_MS_ASM = string("_asm");
const string ASResource::AS_MS__ASM = string("__asm");

const string ASResource::AS_BAR_DEFINE = string("#define");
const string ASResource::AS_BAR_INCLUDE = string("#include");
const string ASResource::AS_BAR_IF = string("#if");
const string ASResource::AS_BAR_EL = string("#el");
const string ASResource::AS_BAR_ENDIF = string("#endif");

const string ASResource::AS_OPEN_BRACKET = string("{");
const string ASResource::AS_CLOSE_BRACKET = string("}");
const string ASResource::AS_OPEN_LINE_COMMENT = string("//");
const string ASResource::AS_OPEN_COMMENT = string("/*");
const string ASResource::AS_CLOSE_COMMENT = string("*/");

const string ASResource::AS_ASSIGN = string("=");
const string ASResource::AS_PLUS_ASSIGN = string("+=");
const string ASResource::AS_MINUS_ASSIGN = string("-=");
const string ASResource::AS_MULT_ASSIGN = string("*=");
const string ASResource::AS_DIV_ASSIGN = string("/=");
const string ASResource::AS_MOD_ASSIGN = string("%=");
const string ASResource::AS_OR_ASSIGN = string("|=");
const string ASResource::AS_AND_ASSIGN = string("&=");
const string ASResource::AS_XOR_ASSIGN = string("^=");
const string ASResource::AS_GR_GR_ASSIGN = string(">>=");
const string ASResource::AS_LS_LS_ASSIGN = string("<<=");
const string ASResource::AS_GR_GR_GR_ASSIGN = string(">>>=");
const string ASResource::AS_LS_LS_LS_ASSIGN = string("<<<=");
const string ASResource::AS_GCC_MIN_ASSIGN = string("<?");
const string ASResource::AS_GCC_MAX_ASSIGN = string(">?");

const string ASResource::AS_RETURN = string("return");
const string ASResource::AS_CIN = string("cin");
const string ASResource::AS_COUT = string("cout");
const string ASResource::AS_CERR = string("cerr");

const string ASResource::AS_EQUAL = string("==");
const string ASResource::AS_PLUS_PLUS = string("++");
const string ASResource::AS_MINUS_MINUS = string("--");
const string ASResource::AS_NOT_EQUAL = string("!=");
const string ASResource::AS_GR_EQUAL = string(">=");
const string ASResource::AS_GR_GR = string(">>");
const string ASResource::AS_GR_GR_GR = string(">>>");
const string ASResource::AS_LS_EQUAL = string("<=");
const string ASResource::AS_LS_LS = string("<<");
const string ASResource::AS_LS_LS_LS = string("<<<");
const string ASResource::AS_QUESTION_QUESTION = string("??");
const string ASResource::AS_LAMBDA = string("=>");            // C# lambda expression arrow
const string ASResource::AS_ARROW = string("->");
const string ASResource::AS_AND = string("&&");
const string ASResource::AS_OR = string("||");
const string ASResource::AS_SCOPE_RESOLUTION = string("::");

const string ASResource::AS_PLUS = string("+");
const string ASResource::AS_MINUS = string("-");
const string ASResource::AS_MULT = string("*");
const string ASResource::AS_DIV = string("/");
const string ASResource::AS_MOD = string("%");
const string ASResource::AS_GR = string(">");
const string ASResource::AS_LS = string("<");
const string ASResource::AS_NOT = string("!");
const string ASResource::AS_BIT_OR = string("|");
const string ASResource::AS_BIT_AND = string("&");
const string ASResource::AS_BIT_NOT = string("~");
const string ASResource::AS_BIT_XOR = string("^");
const string ASResource::AS_QUESTION = string("?");
const string ASResource::AS_COLON = string(":");
const string ASResource::AS_COMMA = string(",");
const string ASResource::AS_SEMICOLON = string(";");

const string ASResource::AS_QFOREACH = string("Q_FOREACH");
const string ASResource::AS_QFOREVER = string("Q_FOREVER");
const string ASResource::AS_FOREVER = string("forever");
const string ASResource::AS_FOREACH = string("foreach");
const string ASResource::AS_LOCK = string("lock");
const string ASResource::AS_UNSAFE = string("unsafe");
const string ASResource::AS_FIXED = string("fixed");
const string ASResource::AS_GET = string("get");
const string ASResource::AS_SET = string("set");
const string ASResource::AS_ADD = string("add");
const string ASResource::AS_REMOVE = string("remove");
const string ASResource::AS_DELEGATE = string("delegate");
const string ASResource::AS_UNCHECKED = string("unchecked");

const string ASResource::AS_CONST_CAST = string("const_cast");
const string ASResource::AS_DYNAMIC_CAST = string("dynamic_cast");
const string ASResource::AS_REINTERPRET_CAST = string("reinterpret_cast");
const string ASResource::AS_STATIC_CAST = string("static_cast");

const string ASResource::AS_NS_DURING = string("NS_DURING");
const string ASResource::AS_NS_HANDLER = string("NS_HANDLER");

}   // end of astyle namespace
