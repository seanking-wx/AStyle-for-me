#include "ASStreamIterator.h"

namespace AStyle
{

//-----------------------------------------------------------------------------
// ASStreamIterator class
// typename will be istringstream for GUI and istream otherwise
//-----------------------------------------------------------------------------

template<typename T>
ASStreamIterator<T>::ASStreamIterator(T* in)
{
	inStream = in;
	buffer.reserve(200);
	eolWindows = 0;
	eolLinux = 0;
	eolMacOld = 0;
	outputEOL[0] = '\0';
	peekStart = 0;
	prevLineDeleted = false;
	checkForEmptyLine = false;
	// get length of stream
	inStream->seekg(0, inStream->end);
	streamLength = inStream->tellg();
	inStream->seekg(0, inStream->beg);
}

template<typename T>
ASStreamIterator<T>::~ASStreamIterator()
{
}

/**
* get the length of the input stream.
* streamLength variable is set by the constructor.
*
* @return     length of the input file stream, converted to an int.
*/
template<typename T>
int ASStreamIterator<T>::getStreamLength() const
{
	return static_cast<int>(streamLength);
}

/**
 * read the input stream, delete any end of line characters,
 *     and build a string that contains the input line.
 *
 * @return        string containing the next input line minus any end of line characters
 */
template<typename T>
string ASStreamIterator<T>::nextLine(bool emptyLineWasDeleted)
{
	// verify that the current position is correct
	assert(peekStart == 0);
	// a deleted line may be replaced if break-blocks is requested
	// this sets up the compare to check for a replaced empty line
	if(prevLineDeleted)
	{
		prevLineDeleted = false;
		checkForEmptyLine = true;
	}
	if(!emptyLineWasDeleted)
		prevBuffer = buffer;
	else
		prevLineDeleted = true;
	// read the next record
	buffer.clear();
	char ch;
	inStream->get(ch);
	while(!inStream->eof() && ch != '\n' && ch != '\r')
	{
		buffer.append(1, ch);
		inStream->get(ch);
	}
	if(inStream->eof())
		return buffer;
	int peekCh = inStream->peek();
	// find input end-of-line characters
	if(!inStream->eof())
	{
		if(ch == '\r')          // CR+LF is windows otherwise Mac OS 9
		{
			if(peekCh == '\n')
			{
				inStream->get();
				eolWindows++;
			}
			else
				eolMacOld++;
		}
		else                    // LF is Linux, allow for improbable LF/CR
		{
			if(peekCh == '\r')
			{
				inStream->get();
				eolWindows++;
			}
			else
				eolLinux++;
		}
	}
	else
		inStream->clear();
	// set output end of line characters
	if(eolWindows >= eolLinux)
	{
		if(eolWindows >= eolMacOld)
			strcpy(outputEOL, "\r\n");  // Windows (CR+LF)
		else
			strcpy(outputEOL, "\r");    // MacOld (CR)
	}
	else if(eolLinux >= eolMacOld)
		strcpy(outputEOL, "\n");        // Linux (LF)
	else
		strcpy(outputEOL, "\r");        // MacOld (CR)
	return buffer;
}

// save the current position and get the next line
// this can be called for multiple reads
// when finished peeking you MUST call peekReset()
// call this function from ASFormatter ONLY
template<typename T>
string ASStreamIterator<T>::peekNextLine()
{
	assert(hasMoreLines());
	string nextLine_;
	char ch;
	if(peekStart == 0)
		peekStart = inStream->tellg();
	// read the next record
	inStream->get(ch);
	while(!inStream->eof() && ch != '\n' && ch != '\r')
	{
		nextLine_.append(1, ch);
		inStream->get(ch);
	}
	if(inStream->eof())
		return nextLine_;
	int peekCh = inStream->peek();
	// remove end-of-line characters
	if(!inStream->eof())
	{
		if((peekCh == '\n' || peekCh == '\r') && peekCh != ch)
			inStream->get();
	}
	return nextLine_;
}

// reset current position and EOF for peekNextLine()
template<typename T>
void ASStreamIterator<T>::peekReset()
{
	assert(peekStart != 0);
	inStream->clear();
	inStream->seekg(peekStart);
	peekStart = 0;
}

// save the last input line after input has reached EOF
template<typename T>
void ASStreamIterator<T>::saveLastInputLine()
{
	assert(inStream->eof());
	prevBuffer = buffer;
}

// return position of the get pointer
template<typename T>
streamoff ASStreamIterator<T>::tellg()
{
	return inStream->tellg();
}

// check for a change in line ends
template<typename T>
bool ASStreamIterator<T>::getLineEndChange(int lineEndFormat) const
{
	assert(lineEndFormat == LINEEND_DEFAULT
		   || lineEndFormat == LINEEND_WINDOWS
		   || lineEndFormat == LINEEND_LINUX
		   || lineEndFormat == LINEEND_MACOLD);
	bool lineEndChange = false;
	if(lineEndFormat == LINEEND_WINDOWS)
		lineEndChange = (eolLinux + eolMacOld != 0);
	else if(lineEndFormat == LINEEND_LINUX)
		lineEndChange = (eolWindows + eolMacOld != 0);
	else if(lineEndFormat == LINEEND_MACOLD)
		lineEndChange = (eolWindows + eolLinux != 0);
	else
	{
		if(eolWindows > 0)
			lineEndChange = (eolLinux + eolMacOld != 0);
		else if(eolLinux > 0)
			lineEndChange = (eolWindows + eolMacOld != 0);
		else if(eolMacOld > 0)
			lineEndChange = (eolWindows + eolLinux != 0);
	}
	return lineEndChange;
}

}   // end of astyle namespace
