// Vec4.h By Zsíroskenyér Team 2013.10.09 23:12 last modified Németh Richárd
#pragma once

#include <math.h>
#include <iostream>

class Matrix44;
class Vec3;

class Vec4 {
public:
	// Coordinates
	float x,y,z, w;

	// Ctor
	Vec4();
	Vec4(const Vec3& v, float w);
	Vec4(float x, float y, float z, float w);
	Vec4(float v[4]);

	// Accessors
	float operator [](size_t idx) const;
	float& operator [](size_t idx);

	// Vector stuff
	Vec4& Normalize();
	float Lenght() const;
	float Dot(const Vec4& v) const;

	// Math operators
	Vec4& operator *= (const Vec4& v2);
	Vec4& operator /= (const Vec4& v2);
	Vec4& operator += (const Vec4& v2);
	Vec4& operator -= (const Vec4& v2);

	Vec4& operator *= (float s);
	Vec4& operator /= (float s); 

	Vec4 operator * (const Vec4& v2) const;
	Vec4 operator / (const Vec4& v2) const;
	Vec4 operator + (const Vec4& v2) const;
	Vec4 operator - (const Vec4& v2) const;

	Vec4 operator * (float s) const;
	Vec4 operator / (float s) const;
	friend const Vec4 operator*(float s, Vec4 v);

	Vec4 operator - () const;

	// Matrix trans
	Vec4 operator * (const Matrix44& m) const;
	Vec4& operator *= (const Matrix44& m);
	friend Vec4 operator*(Matrix44 m, const Vec4& v);

	// boolean operators
	bool operator == (const Vec4& v) const;
	bool operator != (const Vec4& v) const;

	// Convserion
	operator Vec3();
};

// global functions
static Vec4 Normalize(const Vec4& v);
static float Dot(const Vec4& v1, const Vec4& v2);
static Vec4 Cross(const Vec4& v1, const Vec4& v2);
static float Lenght(const Vec4& v);

// utility functions ///////////////////////////////
std::ostream& operator<<(std::ostream& os, Vec4 v);




