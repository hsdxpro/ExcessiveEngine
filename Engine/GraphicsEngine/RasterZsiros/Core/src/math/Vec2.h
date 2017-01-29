// Vec2.h By Zsíroskenyér Team 2013.10.10
#pragma once

struct Vec2 {
	Vec2 operator - (const Vec2& p) const;
	Vec2 operator + (const Vec2& p) const;
	Vec2 operator / (const Vec2& p) const;
	Vec2 operator * (const Vec2& p) const;

	Vec2 operator - () const;
	
	Vec2& operator *= (float s);
	Vec2& operator /= (float s);

	bool operator == (const Vec2& p) const;

	Vec2(float X, float Y);
	Vec2();

	float x;
	float y;
};
