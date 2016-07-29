#pragma once

#pragma warning(disable: 4996)  // secure version deprecation warnings

#include <string>
#include <vector>

namespace AStyle
{

using namespace std;

//----------------------------------------------------------------------------
// ASTranslation base class.
//----------------------------------------------------------------------------

// This base class is inherited by the language translation classes.
// Polymorphism is used to call the correct language translator.
// This class contains the translation vector and settext translation method.
// The language vector is built by the language sub classes.
// NOTE: This class must have virtual methods for typeid() to work.
//       typeid() is used by AStyleTestI18n_Localizer.cpp.
class ASTranslation
{
	public:
		ASTranslation() {}
		virtual ~ASTranslation() {}

		string convertToMultiByte(const wstring &wideStr) const;
		size_t getTranslationVectorSize() const;
		bool getWideTranslation(const string &stringIn, wstring &wideOut) const;
		string &translate(const string &stringIn) const;

	protected:
		void addPair(const string &english, const wstring &translated);

	protected:
		vector<pair<string, wstring> > m_translation;       // translation vector
};

class English : public ASTranslation
{
	public:
		English() {}
};

}   // end of astyle namespace

/*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   To ����һ������:
*
*   ���һ���������Զ�Ӧ�����Է�����(�̳���ASTranslation��)�ļ���Translation�ļ���
*   ���������Է�����.h�ļ��б������������Է������Ӧ��.h�ļ�
*   ���Է����๹�캯������addPair��������������Ժ�Ӣ�﷭���ת����
*   �����Ҫ�����º���ASLocalizer::setLanguageFromLCID()�е�WinLangCode���͵ľ�̬����
*   �ں���ASLocalizer::setTranslationClass()����Ӵ����������Է�����Ĵ���
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
//----------------------------------------------------------------------------
// Translation classes
// One class for each language.
// These classes have only a constructor which builds the language vector.
//----------------------------------------------------------------------------
// ���Է�����.h�ļ��б�
#include "ChineseSimplified.h"