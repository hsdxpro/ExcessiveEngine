// Quat.h By Zsíroskenyér Team 2013.10.09 23:52 last modified Németh Richárd
#pragma once

#include <math.h>
#include "Vec3.h"

class Matrix44;
class Quat {
public:
	float x,y,z,w;

	Quat();
	Quat(float x, float y, float z, float w);
	Quat(const Vec3& v, float angle);
	Quat(const Quat& rhs) = default;
	Quat& operator=(const Quat& rhs) = default;

	// quat properties and ops
	float Lenght() const;
	Quat& Normalize();
	Quat& Conjugate();
		
	// rotation behaviour
	static Quat EulerAnglesToQuat(const Vec3& eulerAngles);
	static Quat EulerAnglesToQuat(float rotX, float rotY, float rotZ);
	static Vec3 QuatToEulerAngles(const Quat& quaternion);
	static Quat	DirToRot(const Vec3& dir, const Vec3& up);

	// interaction with Vec3's
	static Vec3 RotateVec3(Vec3 v, Quat q);
	static Vec3 RotateVec3_2(Vec3 v, Quat q);
	friend Vec3 operator*(const Vec3& v, const Quat& q);
	Vec3 operator*(const Vec3& v);
	friend Vec3& operator*=(Vec3& v, const Quat& q);

	// operators
	Quat& operator*=(const Quat& q2);	// quat miltiply
	Quat operator*(const Quat& q2) const;
	const Quat operator~() const;		// conjugate

	bool operator==(const Quat& r) const;	// compare
	bool operator!=(const Quat& r) const;

	// type casting to rotation matrix
	operator Matrix44();
};

Quat Normalize(const Quat& q);
float Lenght(const Quat& q);
float Dot(const Quat& q1, const Quat& q2);

// Utility
std::ostream& operator<<(std::ostream& os, Quat q);





