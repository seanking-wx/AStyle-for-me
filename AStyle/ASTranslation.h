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
*   To 新增一种语言:
*
*   添加一个新增语言对应的语言翻译类(继承自ASTranslation类)文件到Translation文件夹
*   在下面语言翻译类.h文件列表后包含新增语言翻译类对应的.h文件
*   语言翻译类构造函数中用addPair函数添加新增语言和英语翻译的转换对
*   如果需要，更新函数ASLocalizer::setLanguageFromLCID()中的WinLangCode类型的静态数组
*   在函数ASLocalizer::setTranslationClass()中添加创建新增语言翻译类的代码
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
//----------------------------------------------------------------------------
// Translation classes
// One class for each language.
// These classes have only a constructor which builds the language vector.
//----------------------------------------------------------------------------
// 语言翻译类.h文件列表
#include "ChineseSimplified.h"