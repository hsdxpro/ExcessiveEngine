//StrUtil.h By Zsíroskenyér Team 2014.01.11 20:19
#pragma once

#include "zsString.h"
#include <vector>
#include <list>
#include <fstream>
#include <sstream>

//#include <boost/mpl/numeric_cast.hpp>

class cStrUtil {
public:
	static void ToAnsi(const zsString& src, char* dst, size_t nChars);
	static size_t ToUnsigned(const zsString& str);
	static void ToUpper(std::list<zsString>& strs);

	static bool Contains(const zsString& in, const zsString& that);
	static bool Contains(const std::list<zsString>& strs, const zsString& str);

	// returns - 1 : string not found
	// return >= 0 : index of the founded str in zsString
	static int Find(const zsString& in, const zsString& that);

	// returns - 1 : string not found
	// return >= 0 : index of the founded str in zsString
	static int Find(const zsString& in, wchar_t that);
	static int Find(const char* in, char that, char rightBound);

	// ... Find str in str, when reach rightBound stop search
	static int Find(const char* in, const char* that, char rightBound);

	static int FindLeft(const zsString& in, int startPos, wchar_t that);
	static int FindLeft(const char* in, int startPos, char that, char leftBound);

	static bool Begins(const zsString& str, const zsString& withThat);

	static zsString CutBack(const zsString& str, wchar_t ch);
	static void		CutBack(zsString& str, wchar_t ch);

	static zsString CutDirectory(const zsString& str);
	static void CutNumberFromEnd(char* src);
	
	static void CutFrontFromDelim(zsString& strOut, wchar_t ch);

	static void TrimBorder(std::list<zsString>& strs, const wchar_t* borderChars, size_t nChars);
	static void TrimBorder(zsString& strOut, wchar_t borderChar);

	static char const* NextLine(char const*  p);

	// The function extract last T typed number like '5' from str, need tmp char buffer for that (chars_out, nChars)
	template<class T>
	static void LastNumber(const zsString& str, T& out, wchar_t* char_out, uint32_t nChars);

	template<class T>
	static void LastNumber(const char* str, T& out, std::vector<char>& chars_out, char rightBound);

	// Gather string between left and right characters, for ex. zsString ex = _asdasd; ex.Between('-',';') returns asdasd   
	static void Between(zsString& strOut, wchar_t left, wchar_t right);

	// Gather string between left and right strings, for ex. zsString ex = _asdasd; ex.Between('-',';') returns asdasd   
	static void Between(zsString& strOut, const wchar_t* left, const wchar_t* right);

	static void Between(zsString& strOut, wchar_t left, const wchar_t* rightDelims, size_t nRightDelims);

	static zsString SubStrLeft(const zsString& str, size_t startPos, wchar_t leftBound, size_t leftCutOffset = 0);
	static zsString SubStrLeft(const char* str, size_t startPos, char leftBound, size_t leftCutOffset = 0);
	static zsString SubStrLeft(const zsString& str, size_t startPos);
	

	static zsString SubStrRight(const zsString& str, size_t startPos, wchar_t rightBound, size_t rightCutOffset = 0);

	template<size_t nBounds>
	static zsString SubStrRight(const char* str, size_t startPos, char (&leftBounds)[nBounds], size_t rightCutOffset = 0) {
		return zsString();
	}

	static zsString SubStrRight(const zsString& str, size_t pos);

	// Split string into two parts at (ch) character (chleaved)
	static std::list<zsString> SplitAt(const zsString& str, wchar_t ch);

	// Gather string between left and right characters, for ex. zsString ex = _asdasd; ex.Between('-',';') returns asdasd   
	static zsString Between(const zsString& s, wchar_t left, wchar_t right);  
	static zsString Between(const char* p, char left, char right, char rightBound);
	static zsString Between(const zsString& s, const wchar_t* left, const wchar_t* right);
	static zsString Between(const zsString& s, wchar_t left, const wchar_t* rightDelims, size_t nRightDelims);

	

	static std::list<size_t> GetLines(const std::list<zsString>& strs, size_t startLineIdx, const zsString& containStr);
	static void GetNumberFromEnd(char* src, char* buf_out);
	static void GetWordBetween(zsString& str, char left, char right, char* buf_out);
	static void GetFloats(const zsString& str, std::vector<float>& floats_out);
	static zsString GetDirectory(const zsString& str);
	static zsString GetWordAfter(const std::list<zsString>& strs, const zsString& str);
	static std::list<zsString> GetLinesBetween(const std::list<zsString>& strs, const zsString& str, const zsString& endLine);
	static std::list<zsString> GetLinesBeginsWith(const std::list<zsString>& strs, const zsString& str);
	static std::list<size_t> GetLinesContainingAllStr(const std::list<zsString>& in, const zsString* those, size_t nThose);
};

template<class T>
static void cStrUtil::LastNumber(const zsString& str, T& out, wchar_t* char_out, uint32_t nChars) {
	assert(nChars > 1); // buffer for leaast one 0wchar_t + null terminator

	// Go to back, search for digit
	const wchar_t* s = str.c_str();
	while (*s != '\0')
		*s++;

	// Search for totally last digit
	size_t lastDigitIdx = str.size();
	while ( ! isdigit(*s)) {
		s--;
		lastDigitIdx--;
	}
	
	// Search for continuous digits
	uint32_t firstDigitIdx = lastDigitIdx;

	s--;
	while (isdigit(*s) && s != (str.c_str() - 1)) {
		s--;
		firstDigitIdx--;
	}

	// 1 char
	if (firstDigitIdx == lastDigitIdx) {
		char_out[0] = str.c_str()[lastDigitIdx];
		char_out[1] = '\0'; // Terminate
	} else {
		uint32_t size = lastDigitIdx - firstDigitIdx + 1;
		assert(size <= nChars);

		memcpy(char_out, str.c_str() + firstDigitIdx, sizeof(wchar_t) * size);
		char_out[size] = '\0';
	}
	
	// TODO FUCK STRINGSTREAM FUCK....replace with BOOST as SOON as POSSIBLE
	std::wstringstream ss;
	ss << char_out;
	ss >> out;
}

template<class T>
static void cStrUtil::LastNumber(const char* str, T& out, std::vector<char>& chars_out, char rightBound) {
	assert(chars_out.size() > 1); // buffer for leaast one char + null terminator

	// Go to back, search for digit
	const char* s = str;
	while (*s != '\0')
		*s++;

	// Search for totally last digit
	uint32_t lastDigitIdx = s - str;
	while (!isdigit(*s)) {
		s--;
		lastDigitIdx--;
	}

	// Search for continuous digits
	uint32_t firstDigitIdx = lastDigitIdx;

	s--;
	while (isdigit(*s) && s != (str - 1)) {
		s--;
		firstDigitIdx--;
	}

	// 1 char
	if (firstDigitIdx == lastDigitIdx) {
		chars_out[0] = str[lastDigitIdx];
		chars_out[1] = '\0'; // Terminate
	}
	else {
		uint32_t size = lastDigitIdx - firstDigitIdx + 1;
		assert(size <= chars_out.size());

		memcpy(chars_out.data(), str + firstDigitIdx, size);
		chars_out[size] = '\0';
	}

	// TODO FUCK STRINGSTREAM FUCK....replace with BOOST as SOON as POSSIBLE
	std::stringstream ss;
	ss << chars_out.data();
	ss >> out;
}