#include <Windows.h>
#include <cassert>
#include "ASLocalizer.h"

namespace AStyle
{

//----------------------------------------------------------------------------
// ASLocalizer class methods.
//----------------------------------------------------------------------------

// Set the locale information.
ASLocalizer::ASLocalizer()
{
	// set language default values to english (ascii)
	// this will be used if a locale or a language cannot be found
	m_localeName = "UNKNOWN";
	m_langID = "en";
	m_lcid = 0;
	m_subLangID.clear();
	m_translation = NULL;
	// Not all compilers support the C++ function locale::global(locale(""));
	// For testing on Windows change the "Region and Language" settings or use AppLocale.
	// For testing on Linux change the LANG environment variable: LANG=fr_FR.UTF-8.
	// setlocale() will use the LANG environment variable on Linux.
	char* localeName = setlocale(LC_ALL, "");
	if(localeName == NULL)      // use the english (ascii) defaults
	{
		fprintf(stderr, "\n%s\n\n", "Cannot set native locale, reverting to English");
		setTranslationClass();
		return;
	}
	// set the class variables
#ifdef _WIN32
	size_t lcid = GetUserDefaultLCID();
	setLanguageFromLCID(lcid);
#else
	setLanguageFromName(localeName);
#endif
}

// Delete dynamically allocated memory.
ASLocalizer::~ASLocalizer()
{
	delete m_translation;
}

#ifdef _WIN32

// Windows get the language to use from the user locale.
// NOTE: GetUserDefaultLocaleName() gets nearly the same name as Linux.
//       But it needs Windows Vista or higher.
//       Same with LCIDToLocaleName().
void ASLocalizer::setLanguageFromLCID(size_t lcid)
{
	m_lcid = lcid;
	m_langID = "en";    // default to english
	size_t lang = PRIMARYLANGID(LANGIDFROMLCID(m_lcid));
	size_t sublang = SUBLANGID(LANGIDFROMLCID(m_lcid));
	// primary language identifier http://msdn.microsoft.com/en-us/library/aa912554.aspx
	// sublanguage identifier http://msdn.microsoft.com/en-us/library/aa913256.aspx
	// language ID http://msdn.microsoft.com/en-us/library/ee797784%28v=cs.20%29.aspx
	static WinLangCode wlc[] =
	{
		{ LANG_CHINESE,    "zh" },
		{ LANG_DUTCH,      "nl" },
		{ LANG_ENGLISH,    "en" },
		{ LANG_FINNISH,    "fi" },
		{ LANG_FRENCH,     "fr" },
		{ LANG_GERMAN,     "de" },
		{ LANG_HINDI,      "hi" },
		{ LANG_ITALIAN,    "it" },
		{ LANG_JAPANESE,   "ja" },
		{ LANG_KOREAN,     "ko" },
		{ LANG_POLISH,     "pl" },
		{ LANG_PORTUGUESE, "pt" },
		{ LANG_RUSSIAN,    "ru" },
		{ LANG_SPANISH,    "es" },
		{ LANG_SWEDISH,    "sv" },
		{ LANG_UKRAINIAN,  "uk" },
	};
	// find language in the wlc table
	size_t count = sizeof(wlc) / sizeof(wlc[0]);
	for(size_t i = 0; i < count; i++)
	{
		if(wlc[i].winLang == lang)
		{
			m_langID = wlc[i].canonicalLang;
			break;
		}
	}
	if(m_langID == "zh")
	{
		if(sublang == SUBLANG_CHINESE_SIMPLIFIED
		   || sublang == SUBLANG_CHINESE_SINGAPORE)
			m_subLangID = "CHS";
		else
			m_subLangID = "CHT";    // default
	}
	setTranslationClass();
}
#endif  // _win32

// Linux set the language to use from the langID.
//
// the language string has the following form
//
//      lang[_LANG][.encoding][@modifier]
//
// (see environ(5) in the Open Unix specification)
//
// where lang is the primary language, LANG is a sublang/territory,
// encoding is the charset to use and modifier "allows the user to select
// a specific instance of localization data within a single category"
//
// for example, the following strings are valid:
//      fr
//      fr_FR
//      de_DE.iso88591
//      de_DE@euro
//      de_DE.iso88591@euro
void ASLocalizer::setLanguageFromName(const char* langID)
{
	// the constants describing the format of lang_LANG locale string
	static const size_t LEN_LANG = 2;
	m_lcid = 0;
	string langStr = langID;
	m_langID = langStr.substr(0, LEN_LANG);
	// need the sublang for chinese
	if(m_langID == "zh" && langStr[LEN_LANG] == '_')
	{
		string subLang = langStr.substr(LEN_LANG + 1, LEN_LANG);
		if(subLang == "CN" || subLang == "SG")
			m_subLangID = "CHS";
		else
			m_subLangID = "CHT";    // default
	}
	setTranslationClass();
}

// Call the settext class and return the value.
const char* ASLocalizer::settext(const char* textIn) const
{
	assert(m_translation);
	const string stringIn = textIn;
	return m_translation->translate(stringIn).c_str();
}

// Return the required translation class.
// Sets the class variable m_translation from the value of m_langID.
// Get the language ID at http://msdn.microsoft.com/en-us/library/ee797784%28v=cs.20%29.aspx
void ASLocalizer::setTranslationClass()
{
	assert(m_langID.length());
	// delete previously set (--ascii option)
	if(m_translation)
	{
		delete m_translation;
		m_translation = NULL;
	}
	if(m_langID == "en")
		m_translation = new English;
	else if(m_langID == "zh" && m_subLangID == "CHS")
		m_translation = new ChineseSimplified;
#if 0
	else if(m_langID == "zh" && m_subLangID == "CHT")
		m_translation = new ChineseTraditional;
	else if(m_langID == "nl")
		m_translation = new Dutch;
	else if(m_langID == "en")
		m_translation = new English;
	else if(m_langID == "fi")
		m_translation = new Finnish;
	else if(m_langID == "fr")
		m_translation = new French;
	else if(m_langID == "de")
		m_translation = new German;
	else if(m_langID == "hi")
		m_translation = new Hindi;
	else if(m_langID == "it")
		m_translation = new Italian;
	else if(m_langID == "ja")
		m_translation = new Japanese;
	else if(m_langID == "ko")
		m_translation = new Korean;
	else if(m_langID == "pl")
		m_translation = new Polish;
	else if(m_langID == "pt")
		m_translation = new Portuguese;
	else if(m_langID == "ru")
		m_translation = new Russian;
	else if(m_langID == "es")
		m_translation = new Spanish;
	else if(m_langID == "sv")
		m_translation = new Swedish;
	else if(m_langID == "uk")
		m_translation = new Ukrainian;
#endif
	else    // default
		m_translation = new English;
}

}   // end of astyle namespace
