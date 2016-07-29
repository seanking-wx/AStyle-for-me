#pragma once

namespace AStyle
{

//----------------------------------------------------------------------------
// Utf8_16 class for utf8/16 conversions
//----------------------------------------------------------------------------

class Utf8_16
{
	private:
		typedef unsigned short utf16; // 16 bits
		typedef unsigned char utf8;   // 8 bits
		typedef unsigned char ubyte;  // 8 bits
		enum
		{
			SURROGATE_LEAD_FIRST = 0xD800,
			SURROGATE_LEAD_LAST = 0xDBFF,
			SURROGATE_TRAIL_FIRST = 0xDC00,
			SURROGATE_TRAIL_LAST = 0xDFFF,
			SURROGATE_FIRST_VALUE = 0x10000
		};
		enum eState
		{
			eStart,
			eSecondOf4Bytes,
			ePenultimate,
			eFinal
		};
	public:
		Utf8_16(void) {}
		virtual ~Utf8_16(void) {}

		bool getBigEndian() const;
		int swap16bit(int value) const;
		size_t utf16len(const utf16* utf16In) const;
		size_t Utf8LengthFromUtf16(const char* utf16In, size_t inLen, bool isBigEndian) const;
		size_t Utf8ToUtf16(char* utf8In, size_t inLen, bool isBigEndian, char* utf16Out) const;
		size_t Utf16LengthFromUtf8(const char* utf8In, size_t inLen) const;
		size_t Utf16ToUtf8(char* utf16In, size_t inLen, bool isBigEndian, bool firstBlock, char* utf8Out) const;
};

}   // end of astyle namespace
