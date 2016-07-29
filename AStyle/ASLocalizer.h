#pragma once
#include "ASTranslation.h"
namespace AStyle
{

#ifdef _WIN32
struct WinLangCode
{
	size_t winLang;
	char canonicalLang[3];
};
#endif

//-----------------------------------------------------------------------------
// ASLocalizer class for console build.
// This class encapsulates all language-dependent settings and is a
// generalization of the C locale concept.
//-----------------------------------------------------------------------------

class ASLocalizer
{
	public:
		ASLocalizer(void);
		virtual ~ASLocalizer(void);

		// Returns the language ID in m_langID.
		string getLanguageID() const
		{
			return m_langID;
		}
		// Returns the name of the translation class in m_translation.  Used for testing.
		const ASTranslation* getTranslationClass() const
		{
			assert(m_translation);
			return m_translation;
		}
		void setLanguageFromName(const char* langID);
		const char* settext(const char* textIn) const;

	private:    // functions
#ifdef _WIN32
		void setLanguageFromLCID(size_t lcid);
#endif
		void setTranslationClass();

	private:    // variables
		ASTranslation* m_translation;     // pointer to a polymorphic Translation class
		string m_langID;                // language identifier from the locale
		string m_subLangID;             // sub language identifier, if needed
		string m_localeName;            // name of the current locale (Linux only)
		size_t m_lcid;                  // LCID of the user locale (Windows only)
};

}   // end of astyle namespace
