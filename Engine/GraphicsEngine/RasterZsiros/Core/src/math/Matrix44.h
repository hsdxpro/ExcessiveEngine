// Matrix44.h By Zsíroskenyér Engine 2013.10.09 23:17
// Homogeneous matrix 4x4
#pragma once

#include <cmath>
#include <iostream>
#include "Vec4.h"

class Quat;
class Vec3;

/* what a matrix has to do? 
(-decompose: get scale, rot, and translation from a matrix)
-determinant - IMPLEMENTED
-inverse - IMPLEMENTED (optimization tests maybe)
-multiply
(-reflect: mirrors coordinate system about a plane)
-rotation matrices: from quat, around x,y,z axis by w angle, optionally w angles around a random axis (it's what quats do)
-scaling matrix
-translation matrix
-transpose

useful combinations of the above:
-full transformation, mTransformation = mScale*mRot*mTranslation
*/

class Matrix44 {
public:
	// Data, accessible multiple ways
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		struct {
			Vec4 r[4];
		};
		float m[4][4];
	};

	// Ctors
	Matrix44();
	Matrix44(	float _11, float _12, float _13, float _14,
				float _21, float _22, float _23, float _24,
				float _31, float _32, float _33, float _34,
				float _41, float _42, float _43, float _44);

	// Matrix stuff
	Matrix44& Identity();
	Matrix44& Inverse();
	Matrix44& Transpose();
	float Determinant();

	// Transformations
	// create
	Matrix44& RotationX(float angle);
	Matrix44& RotationY(float angle);
	Matrix44& RotationZ(float angle);
	Matrix44& RotationQuat(const Quat& q);
	Matrix44& RotationEuler(const Vec3& rot);
	Matrix44& RotationAxisAngle(const Vec3& axis, float angle);

	Matrix44& Translation(const Vec3& v);
	Matrix44& Scaling(const Vec3& s);

	Matrix44& ViewRH(const Vec3& eye, const Vec3& target, const Vec3& up);
	Matrix44& ProjPerspectiveRH(float nearPlane, float farPlane, float fovRad, float aspectRatio);
	Matrix44& ProjPerspectiveLH(float nearPlane, float farPlane, float fovRad, float aspectRatio);
	Matrix44& ProjOrtographic(float nearPlane, float farPlane, float left, float right, float bottom, float top);

	// pre
	Matrix44& PreRotate(const Quat& q);

	Matrix44& PreScale(float x, float y, float z);
	Matrix44& PreScale(const Vec3& s);

	Matrix44& PreTranslate(float x, float y, float z);
	Matrix44& PreTranslate(const Vec3& t);
	// post
	Matrix44& PostRotate(const Quat& q);

	Matrix44& PostScale(float x, float y, float z);
	Matrix44& PostScale(const Vec3& s);

	Matrix44& PostTranslate(float x, float y, float z);
	Matrix44& PostTranslate(const Vec3& t);

	// Accessors
	float& operator() (unsigned row, unsigned col);
	const float& operator() (unsigned row, unsigned col) const;

	Vec4& operator[](unsigned idx);
	const Vec4& operator[](unsigned idx) const;
	
	// Operators
	Matrix44& operator *= (const Matrix44& m2);
	Matrix44& operator += (const Matrix44& m2);
	Matrix44& operator -= (const Matrix44& m2);

	Matrix44& operator *= (const float&);
	Matrix44& operator /= (const float&);

	Matrix44 operator * (const Matrix44& m2) const;
	Matrix44 operator + (const Matrix44& m2) const;
	Matrix44 operator - (const Matrix44& m2) const;

	Matrix44 operator * (const float&) const;
	Matrix44 operator / (const float&) const;

	// Compare matrices
	bool operator == (const Matrix44& m2) const;
	bool operator != (const Matrix44& m2) const;
};

// Utility
std::ostream& operator << (std::ostream& os, const Matrix44& m);

// GLobals
Matrix44 lerp(const Matrix44& m1, const Matrix44& m2, float t);


//-------------GLOBALS---------------///

Matrix44 Matrix44Inverse(const Matrix44& in);
Matrix44 Matrix44Transpose(const Matrix44& in);

Matrix44 Matrix44RotationAxisAngle(const Vec3& axis, float angle);
Matrix44 Matrix44RotationQuat(const Quat&); // typecast ((Matrix44)quaternion) is possibly slower than this shit
Matrix44 Matrix44RotationEuler(const Vec3& rot);
Matrix44 Matrix44RotationEuler(float x, float y, float z);
Matrix44 Matrix44RotationX(float angle);
Matrix44 Matrix44RotationY(float angle);
Matrix44 Matrix44RotationZ(float angle);

Matrix44 Matrix44Translation(float x, float y, float z);
Matrix44 Matrix44Translation(const Vec3& v);

Matrix44 Matrix44Scaling(float x, float y, float z);
Matrix44 Matrix44Scaling(const Vec3& v);

Matrix44 Matrix44ViewRH(const Vec3& eye, const Vec3& target, const Vec3& up);
Matrix44 Matrix44ProjPerspectiveRH(float nearPlane, float farPlane, float fovRad, float aspectRatio);
Matrix44 Matrix44ProjPerspectiveLH(float nearPlane, float farPlane, float fovRad, float aspectRatio);
Matrix44 Matrix44ProjOrtographic(float nearPlane, float farPlane, float left, float right, float bottom, float top);


