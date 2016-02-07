// zsString.h By Zsíroskenyér Team 2013.10.10 11:07
#pragma once
#pragma warning(disable: 4996)

#include <sstream>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <functional>

#undef max

// zsString type
typedef std::wstring zsBasicString;


class zsString : public zsBasicString 
{
public:
	// constructors
	zsString() : zsBasicString() {};
	zsString(size_type s, value_type c) : zsBasicString(s, c) {};
	zsString(const zsBasicString& other, size_type pos, size_type count = npos) : zsBasicString(other, pos, count) {}
	zsString(const value_type* s, size_type count) : zsBasicString(s, count) {}
	zsString(const value_type* s) : zsBasicString(s) {}

	template <class InputIt>
	zsString(InputIt first, InputIt last) : zsBasicString(first, last) {}

	zsString(const zsBasicString& other) : zsBasicString(other) {}
	zsString(zsBasicString&& other) : zsBasicString(other) {}

	// Construct from numbers
	//template<class ValType>
	zsString(const std::string& str)
	{
		*this = std::wstring(str.begin(), str.end());
	}

	zsString(const char* str)
	{
		std::string tmp = str;
		*this = std::wstring(tmp.begin(), tmp.end());
	}

	zsString(size_t val) {
		std::wstringstream ss;

		//if (val)
		ss << val;

		auto str = ss.str();
		for (size_t i = 0; i < str.length(); i++)
			zsBasicString::operator += (str[i]);
	}

	// operators
	zsString& operator = (const zsBasicString& str) { zsBasicString::operator=(str); return *this; }
	zsString& operator = (zsBasicString&& str)                { zsBasicString::operator=(str); return *this; }
	zsString& operator = (const value_type* s)                { zsBasicString::operator=(s);         return *this; }
	zsString& operator = (value_type ch)                        { zsBasicString::operator=(ch);  return *this; }

	zsString& operator += (const zsBasicString& str)        { zsBasicString::operator+=(str); return *this; }
	zsString& operator += (value_type ch)                                { zsBasicString::operator+=(ch);  return *this; }
	zsString& operator += (value_type* s)                                { zsBasicString::operator+=(s);          return *this; }

	//template <class ValType>
	zsString operator + (size_t val) { zsString str; str += zsString(val); return str; }
};

template <>
struct std::hash<zsString> {
	size_t operator()(const zsString& str) const {
		return std::hash<zsBasicString>()(str);
	}
};