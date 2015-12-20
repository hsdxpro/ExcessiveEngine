#pragma once
#include "BasicTypes.h"

class Color
{
public:
	inline Color() {}
	inline Color(u8 R, u8 G, u8 B, u8 A) : R(R), G(G), B(B), A(A) {}

	u8 R;
	u8 G;
	u8 B;
	u8 A;

public:
	static Color BLACK;
	static Color WHITE;
	static Color RED;
	static Color GREEN;
	static Color BLUE;
};