#include "ASBase.h"

namespace AStyle
{
// check if a specific line position contains a keyword.
bool ASBase::findKeyword(const string &line, int i, const string &keyword) const
{
	assert(isCharPotentialHeader(line, i));
	// check the word
	const size_t keywordLength = keyword.length();
	const size_t wordEnd = i + keywordLength;
	if(wordEnd > line.length())
		return false;
	if(line.compare(i, keywordLength, keyword) != 0)
		return false;
	// check that this is not part of a longer word
	if(wordEnd == line.length())
		return true;
	if(isLegalNameChar(line[wordEnd]))
		return false;
	// is not a keyword if part of a definition
	const char peekChar = peekNextChar(line, wordEnd - 1);
	if(peekChar == ',' || peekChar == ')')
		return false;
	return true;
}

// get the current word on a line
// index must point to the beginning of the word
string ASBase::getCurrentWord(const string &line, size_t index) const
{
	assert(isCharPotentialHeader(line, index));
	size_t lineLength = line.length();
	size_t i;
	for(i = index; i < lineLength; i++)
	{
		if(!isLegalNameChar(line[i]))
			break;
	}
	return line.substr(index, i - index);
}

}   // end of astyle namespace
