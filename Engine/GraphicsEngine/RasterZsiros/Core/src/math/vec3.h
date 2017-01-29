// Vec3.h By Zsíroskenyér Team 2013.10.09 23:12 last modified Németh Richárd
#pragma once

#include <math.h>
#include <iostream>

class Matrix44;
class Vec4;

class Vec3 {
public:
	// Coordinates
	float x,y,z;

	// Ctors
	Vec3() {x=y=z=0.0f;}
	Vec3(float x, float y, float z) {this->x=x; this->y=y; this->z=z;}
	Vec3(float v[3]) {x = v[0]; y=v[1]; z=v[2];}

	// Vector stuff
	Vec3& Normalize();
	float Lenght() const;
	Vec3& Cross(const Vec3& v);
	float Dot(const Vec3& v) const;

	// Accessors
	float& operator[](size_t idx);
	float operator[](size_t idx) const;

	// Math operators
	Vec3& operator *= (const Vec3& v2);
	Vec3& operator /= (const Vec3& v2);
	Vec3& operator += (const Vec3& v2);
	Vec3& operator -= (const Vec3& v2);

	Vec3& operator *= (float s);
	Vec3& operator /= (float s); 

	Vec3 operator * (const Vec3& v2) const;
	Vec3 operator / (const Vec3& v2) const;
	Vec3 operator + (const Vec3& v2) const;
	Vec3 operator - (const Vec3& v2) const;

	Vec3 operator * (float s) const;
	Vec3 operator / (float s) const;
	friend Vec3 operator*(float s, Vec3 v);

	Vec3 operator - () const;

	// Compare
	bool operator == (const Vec3& w) const;
	bool operator != (const Vec3& w) const;

	// Matrix trans
	Vec3 operator * (const Matrix44& m) const;
	Vec3& operator *= (const Matrix44& m);
	friend Vec3 operator*(Matrix44 m, const Vec3& v);

	// Conversion
	operator Vec4();
};


Vec3 Normalize(const Vec3& v);
float Lenght(const Vec3& v);
float Dot(const Vec3& v1, const Vec3& v2);
Vec3 Cross(const Vec3& v1, const Vec3& v2);

// utility functions ///////////////////////////////
std::ostream& operator<<(std::ostream& os, Vec3 v);

