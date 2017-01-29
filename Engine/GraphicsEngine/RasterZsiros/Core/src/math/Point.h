// Point.h By Zs�roskeny�r Team 2013.10.10 11:16
#pragma once

#include "..\common.h"
#include "Vec2.h"

struct Point {
	
	Point(int X, int Y);

	Point operator - (const Point& p) const;
	Vec2 operator - (const Vec2& p) const;

	Point operator + (const Point& p) const;
	Point operator / (const Point& p) const;
	Point operator * (const Point& p) const;
	
	int x;
	int y;
};
