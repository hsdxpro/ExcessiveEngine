#pragma once
#include "BasicTypes.h"

class Color
{
public:
	inline Color() {}
	inline Color(u8 r, u8 g, u8 b, u8 a) : r(r), g(g), b(b), a(a) {}

	u8 r;
	u8 g;
	u8 b;
	u8 a;

public:
	static Color BLACK;
	static Color WHITE;
	static Color RED;
	static Color GREEN;
	static Color BLUE;
};


#define ENUM_CLASS_BITFLAG( enumClass, enumType )  \
enum class enumClass : enumType; \
class zzzEnum_##enumClass\
{ \
public:\
	zzzEnum_##enumClass(enumClass e) :flag(e) {}\
	operator enumClass() { return flag; }\
	operator bool() { return (enumType)flag != 0; }\
public:\
	enumClass flag;\
};\
inline zzzEnum_##enumClass operator | (enumClass a, enumClass b) \
{\
	return (enumClass)((enumType)a | (enumType)b);\
}\
inline bool operator & (enumClass a, enumClass b)\
{\
	return ((enumType)a & (enumType)b) == 1;\
}\
inline zzzEnum_##enumClass operator ~ (zzzEnum_##enumClass a)\
{\
	return (enumClass)(~(enumType)a.flag);\
}\
inline zzzEnum_##enumClass operator ^ (enumClass a, enumClass b)\
{\
	return (enumClass)((enumType)a ^ (enumType)b);\
}\
inline zzzEnum_##enumClass operator >> (enumClass a, enumClass b)\
{\
	return (enumClass)((enumType)a >> (enumType)b);\
}\
inline zzzEnum_##enumClass operator << (enumClass a, enumClass b)\
{\
	return (enumClass)((enumType)a << (enumType)b);\
}\
inline enumClass& operator |= (enumClass& a, enumClass b)\
{\
	(enumType&)a |= (enumType&)b; \
	return a; \
}\
inline enumClass& operator &= (enumClass& a, enumClass b)\
{\
	(enumType&)a &= (enumType&)b; \
	return a; \
}\
inline enumClass& operator ^= (enumClass& a, enumClass b)\
{\
	(enumType&)a ^= (enumType&)b; \
	return a; \
}\
enum class enumClass : enumType