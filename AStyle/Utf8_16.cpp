#include "Utf8_16.h"

namespace AStyle
{

//----------------------------------------------------------------------------
// Utf8_16 class
//----------------------------------------------------------------------------

// Return true if an int is big endian.
bool Utf8_16::getBigEndian() const
{
	short int word = 0x0001;
	char* byte = (char*) &word;
	return (byte[0] ? false : true);
}

// Swap the two low order bytes of a 16 bit integer value.
int Utf8_16::swap16bit(int value) const
{
	return (((value & 0xff) << 8) | ((value & 0xff00) >> 8));
}

// Return the length of a utf-16 C string.
// The length is in number of utf16_t.
size_t Utf8_16::utf16len(const utf16* utf16In) const
{
	size_t length = 0;
	while(*utf16In++ != '\0')
		length++;
	return length;
}

// Adapted from SciTE UniConversion.cxx.
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// Modified for Artistic Style by Jim Pattee.
// Compute the length of an output utf-8 file given a utf-16 file.
// Input inLen is the size in BYTES (not wchar_t).
size_t Utf8_16::Utf8LengthFromUtf16(const char* utf16In, size_t inLen, bool isBigEndian) const
{
	size_t len = 0;
	size_t wcharLen = inLen / 2;
	const short* uptr = reinterpret_cast<const short*>(utf16In);
	for(size_t i = 0; i < wcharLen && uptr[i];)
	{
		size_t uch = isBigEndian ? swap16bit(uptr[i]) : uptr[i];
		if(uch < 0x80)
			len++;
		else if(uch < 0x800)
			len += 2;
		else if((uch >= SURROGATE_LEAD_FIRST) && (uch <= SURROGATE_TRAIL_LAST))
		{
			len += 4;
			i++;
		}
		else
			len += 3;
		i++;
	}
	return len;
}

// Adapted from SciTE Utf8_16.cxx.
// Copyright (C) 2002 Scott Kirkwood.
// Modified for Artistic Style by Jim Pattee.
// Convert a utf-8 file to utf-16.
size_t Utf8_16::Utf8ToUtf16(char* utf8In, size_t inLen, bool isBigEndian, char* utf16Out) const
{
	int nCur = 0;
	ubyte* pRead = reinterpret_cast<ubyte*>(utf8In);
	utf16* pCur = reinterpret_cast<utf16*>(utf16Out);
	const ubyte* pEnd = pRead + inLen;
	const utf16* pCurStart = pCur;
	eState state = eStart;
	// the BOM will automatically be converted to utf-16
	while(pRead < pEnd)
	{
		switch(state)
		{
			case eStart:
				if((0xF0 & *pRead) == 0xF0)
				{
					nCur = (0x7 & *pRead) << 18;
					state = eSecondOf4Bytes;
				}
				else if((0xE0 & *pRead) == 0xE0)
				{
					nCur = (~0xE0 & *pRead) << 12;
					state = ePenultimate;
				}
				else if((0xC0 & *pRead) == 0xC0)
				{
					nCur = (~0xC0 & *pRead) << 6;
					state = eFinal;
				}
				else
				{
					nCur = *pRead;
					state = eStart;
				}
				break;
			case eSecondOf4Bytes:
				nCur |= (0x3F & *pRead) << 12;
				state = ePenultimate;
				break;
			case ePenultimate:
				nCur |= (0x3F & *pRead) << 6;
				state = eFinal;
				break;
			case eFinal:
				nCur |= (0x3F & *pRead);
				state = eStart;
				break;
		}
		++pRead;
		if(state == eStart)
		{
			int codePoint = nCur;
			if(codePoint >= SURROGATE_FIRST_VALUE)
			{
				codePoint -= SURROGATE_FIRST_VALUE;
				int lead = (codePoint >> 10) + SURROGATE_LEAD_FIRST;
				*pCur++ = static_cast<utf16>(isBigEndian ? swap16bit(lead) : lead);
				int trail = (codePoint & 0x3ff) + SURROGATE_TRAIL_FIRST;
				*pCur++ = static_cast<utf16>(isBigEndian ? swap16bit(trail) : trail);
			}
			else
				*pCur++ = static_cast<utf16>(isBigEndian ? swap16bit(codePoint) : codePoint);
		}
	}
	// return value is the output length in BYTES (not wchar_t)
	return (pCur - pCurStart) * 2;
}

// Adapted from SciTE UniConversion.cxx.
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// Modified for Artistic Style by Jim Pattee.
// Compute the length of an output utf-16 file given a utf-8 file.
// Return value is the size in BYTES (not wchar_t).
size_t Utf8_16::Utf16LengthFromUtf8(const char* utf8In, size_t len) const
{
	size_t ulen = 0;
	size_t charLen;
	for(size_t i = 0; i < len;)
	{
		unsigned char ch = static_cast<unsigned char>(utf8In[i]);
		if(ch < 0x80)
			charLen = 1;
		else if(ch < 0x80 + 0x40 + 0x20)
			charLen = 2;
		else if(ch < 0x80 + 0x40 + 0x20 + 0x10)
			charLen = 3;
		else
		{
			charLen = 4;
			ulen++;
		}
		i += charLen;
		ulen++;
	}
	// return value is the length in bytes (not wchar_t)
	return ulen * 2;
}

// Adapted from SciTE Utf8_16.cxx.
// Copyright (C) 2002 Scott Kirkwood.
// Modified for Artistic Style by Jim Pattee.
// Convert a utf-16 file to utf-8.
size_t Utf8_16::Utf16ToUtf8(char* utf16In, size_t inLen, bool isBigEndian, bool firstBlock, char* utf8Out) const
{
	int nCur16 = 0;
	int nCur = 0;
	ubyte* pRead = reinterpret_cast<ubyte*>(utf16In);
	ubyte* pCur = reinterpret_cast<ubyte*>(utf8Out);
	const ubyte* pEnd = pRead + inLen;
	const ubyte* pCurStart = pCur;
	static eState state = eStart;   // state is retained for subsequent blocks
	if(firstBlock)
		state = eStart;
	// the BOM will automatically be converted to utf-8
	while(pRead < pEnd)
	{
		switch(state)
		{
			case eStart:
				if(pRead >= pEnd)
				{
					++pRead;
					break;
				}
				if(isBigEndian)
				{
					nCur16 = static_cast<utf16>(*pRead++ << 8);
					nCur16 |= static_cast<utf16>(*pRead);
				}
				else
				{
					nCur16 = *pRead++;
					nCur16 |= static_cast<utf16>(*pRead << 8);
				}
				if(nCur16 >= SURROGATE_LEAD_FIRST && nCur16 <= SURROGATE_LEAD_LAST)
				{
					++pRead;
					int trail;
					if(isBigEndian)
					{
						trail = static_cast<utf16>(*pRead++ << 8);
						trail |= static_cast<utf16>(*pRead);
					}
					else
					{
						trail = *pRead++;
						trail |= static_cast<utf16>(*pRead << 8);
					}
					nCur16 = (((nCur16 & 0x3ff) << 10) | (trail & 0x3ff)) + SURROGATE_FIRST_VALUE;
				}
				++pRead;
				if(nCur16 < 0x80)
				{
					nCur = static_cast<ubyte>(nCur16 & 0xFF);
					state = eStart;
				}
				else if(nCur16 < 0x800)
				{
					nCur = static_cast<ubyte>(0xC0 | (nCur16 >> 6));
					state = eFinal;
				}
				else if(nCur16 < SURROGATE_FIRST_VALUE)
				{
					nCur = static_cast<ubyte>(0xE0 | (nCur16 >> 12));
					state = ePenultimate;
				}
				else
				{
					nCur = static_cast<ubyte>(0xF0 | (nCur16 >> 18));
					state = eSecondOf4Bytes;
				}
				break;
			case eSecondOf4Bytes:
				nCur = static_cast<ubyte>(0x80 | ((nCur16 >> 12) & 0x3F));
				state = ePenultimate;
				break;
			case ePenultimate:
				nCur = static_cast<ubyte>(0x80 | ((nCur16 >> 6) & 0x3F));
				state = eFinal;
				break;
			case eFinal:
				nCur = static_cast<ubyte>(0x80 | (nCur16 & 0x3F));
				state = eStart;
				break;
		}
		*pCur++ = static_cast<ubyte>(nCur);
	}
	return pCur - pCurStart;
}

}   // end of astyle namespace
