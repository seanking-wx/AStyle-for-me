#include "ASTranslation.h"

namespace AStyle
{

//----------------------------------------------------------------------------
// Translation base class methods.
//----------------------------------------------------------------------------

// Add a string pair to the translation vector.
void ASTranslation::addPair(const string &english, const wstring &translated)
{
	pair<string, wstring> entry(english, translated);
	m_translation.push_back(entry);
}

// Convert wchar_t to a multibyte string using the currently assigned locale.
// Return an empty string if an error occurs.
string ASTranslation::convertToMultiByte(const wstring &wideStr) const
{
	static bool msgDisplayed = false;
	// get length of the output excluding the NULL and validate the parameters
	size_t mbLen = wcstombs(NULL, wideStr.c_str(), 0);
	if(mbLen == string::npos)
	{
		if(!msgDisplayed)
		{
			fprintf(stderr, "\n%s\n\n", "Cannot convert to multi-byte string, reverting to English");
			msgDisplayed = true;
		}
		return "";
	}
	// convert the characters
	char* mbStr = new(nothrow) char[mbLen + 1];
	if(mbStr == NULL)
	{
		if(!msgDisplayed)
		{
			fprintf(stderr, "\n%s\n\n", "Bad memory alloc for multi-byte string, reverting to English");
			msgDisplayed = true;
		}
		return "";
	}
	wcstombs(mbStr, wideStr.c_str(), mbLen + 1);
	// return the string
	string mbTranslation = mbStr;
	delete [] mbStr;
	return mbTranslation;
}

// Return the translation vector size.  Used for testing.
size_t ASTranslation::getTranslationVectorSize() const
{
	return m_translation.size();
}

// Get the wide translation string. Used for testing.
bool ASTranslation::getWideTranslation(const string &stringIn, wstring &wideOut) const
{
	for(size_t i = 0; i < m_translation.size(); i++)
	{
		if(m_translation[i].first == stringIn)
		{
			wideOut = m_translation[i].second;
			return true;
		}
	}
	// not found
	wideOut = L"";
	return false;
}

// Translate a string.
// Return a static string instead of a member variable so the method can have a "const" designation.
// This allows "settext" to be called from a "const" method.
string &ASTranslation::translate(const string &stringIn) const
{
	static string mbTranslation;
	mbTranslation.clear();
	for(size_t i = 0; i < m_translation.size(); i++)
	{
		if(m_translation[i].first == stringIn)
		{
			mbTranslation = convertToMultiByte(m_translation[i].second);
			break;
		}
	}
	// not found, return english
	if(mbTranslation.empty())
		mbTranslation = stringIn;
	return mbTranslation;
}

}   // end of astyle namespace
