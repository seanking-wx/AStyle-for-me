#pragma once
#include <string>

namespace AStyle
{

using namespace std;

//-----------------------------------------------------------------------------
// Class ASSourceIterator
// A pure virtual class is used by ASFormatter and ASBeautifier instead of
// ASStreamIterator. This allows programs using AStyle as a plug-in to define
// their own ASStreamIterator. The ASStreamIterator class must inherit
// this class.
//-----------------------------------------------------------------------------

class ASSourceIterator
{
	public:
		ASSourceIterator(void) {}
		virtual ~ASSourceIterator(void) {}

		virtual int getStreamLength() const = 0;
		virtual bool hasMoreLines() const = 0;
		virtual string nextLine(bool emptyLineWasDeleted = false) = 0;
		virtual string peekNextLine() = 0;
		virtual void peekReset() = 0;
		virtual streamoff tellg() = 0;
};

}   // end of astyle namespace
