// Color.h By Zsíroskenyér Team 2013.10.10 0:00 last modified Németh Richárd
/////////////////////////////////////////////////////
//	Colorf ---> float4
//	Color ----> DWORD
//	color structs
/////////////////////////////////////////////////////
#pragma once

struct Colorf {
	public:
		float r,g,b,a;

		Colorf();
		Colorf(float m);
		Colorf(float r, float g, float b, float a);
		Colorf(float r, float g, float b);

		float lightness();
		Colorf& tomonochrome();

		Colorf& operator *= (const Colorf& c);
		Colorf& operator /= (const Colorf& c);
		Colorf& operator += (const Colorf& c);
		Colorf& operator -= (const Colorf& c);

		Colorf& operator *= (const float& s);
		Colorf& operator /= (float s);

		Colorf operator * (const Colorf& c);
		Colorf operator / (const Colorf& c);
		Colorf operator + (const Colorf& c);
		Colorf operator - (const Colorf& c);

		Colorf operator * (const float& s);
		Colorf operator / (const float& s);
};