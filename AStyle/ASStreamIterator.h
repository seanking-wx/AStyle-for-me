#pragma once
#include "ASSourceIterator.h"

namespace AStyle
{

//----------------------------------------------------------------------------
// ASStreamIterator class
// typename will be istringstream for GUI and istream otherwise
// ASSourceIterator is an abstract class defined in astyle.h
//----------------------------------------------------------------------------

template<typename T>
class ASStreamIterator : public ASSourceIterator
{
	public:
		ASStreamIterator(T* in);
		virtual ~ASStreamIterator();
	private:
		ASStreamIterator(const ASStreamIterator &copy);       // copy constructor not to be implemented
		ASStreamIterator &operator=(ASStreamIterator &);      // assignment operator not to be implemented

	public:
		bool getLineEndChange(int lineEndFormat) const;
		int  getStreamLength() const;
		string nextLine(bool emptyLineWasDeleted);
		string peekNextLine();
		void peekReset();
		void saveLastInputLine();
		streamoff tellg();

		bool compareToInputBuffer(const string &nextLine_) const
		{
			return (nextLine_ == prevBuffer);
		}
		const char* getOutputEOL() const
		{
			return outputEOL;
		}
		bool hasMoreLines() const
		{
			return !inStream->eof();
		}

	public:
		bool checkForEmptyLine;

	private:
		T* inStream;            // pointer to the input stream
		string buffer;          // current input line
		string prevBuffer;      // previous input line
		int eolWindows;         // number of Windows line endings, CRLF
		int eolLinux;           // number of Linux line endings, LF
		int eolMacOld;          // number of old Mac line endings. CR
		char outputEOL[4];      // next output end of line char
		streamoff streamLength; // length of the input file stream
		streamoff peekStart;    // starting position for peekNextLine
		bool prevLineDeleted;   // the previous input line was deleted
};

}   // end of astyle namespace

