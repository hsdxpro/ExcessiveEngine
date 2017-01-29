////////////////////////////////////////////////////////////////////////////////
// matrix.cpp
// definitions for matrix-stuff
////////////////////////////////////////////////////////////////////////////////
// history:
// 2012.oct.8, monday, 17:57 -> all transform matrices changed to their transpose
//			REASON:							  |a b c d|
//				this is used now:	[x,y,z,1] |e f g h|
//					float1x4				  |i j k l|
//											  |m n o p|
//				this was used:	|x| |a b c d|
//					float4x1	|y| |e f g h|
//								|z| |i j k l|
//								|1| |m n o p|
//			CONSEQUENCE: scale, rotx/y/z, translate should work
//						 quat and axis-angle must be tested
//
// 2012.oct.11, 9:19 -> axis angle matrix rotation doesn't work, fix it

#include "Matrix44.h"
#include "Quat.h"
#include "Vec3.h"
#include "Vec4.h"
#include <memory>

////////////////////////////////////////////////////////////////////////////////
// Constructor
Matrix44::Matrix44() 
{
	this->_11 = 1.0f; this->_12 = 0.0f; this->_13 = 0.0f; this->_14 = 0.0f;
	this->_21 = 0.0f; this->_22 = 1.0f; this->_23 = 0.0f; this->_24 = 0.0f;
	this->_31 = 0.0f; this->_32 = 0.0f; this->_33 = 1.0f; this->_34 = 0.0f;
	this->_41 = 0.0f; this->_42 = 0.0f; this->_43 = 0.0f; this->_44 = 1.0f;
}

Matrix44::Matrix44(	float _11, float _12, float _13, float _14,
					float _21, float _22, float _23, float _24,
					float _31, float _32, float _33, float _34,
					float _41, float _42, float _43, float _44)
{
	this->_11 = _11; this->_12 = (_12); this->_13 = (_13); this->_14 = (_14);
	this->_21 = _21; this->_22 = (_22); this->_23 = (_23); this->_24 = (_24);
	this->_31 = _31; this->_32 = (_32); this->_33 = (_33); this->_34 = (_34);
	this->_41 = _41; this->_42 = (_42); this->_43 = (_43); this->_44 = (_44);
}

////////////////////////////////////////////////////////////////////////////////
// Accessors
float& Matrix44::operator() (unsigned row, unsigned col) {
	return m[row][col];
}

const float& Matrix44::operator() (unsigned row, unsigned col) const {
	return m[row][col];
};

Vec4& Matrix44::operator[](unsigned idx) {
	return r[idx];
}
const Vec4& Matrix44::operator[](unsigned idx) const {
	return r[idx];
}

////////////////////////////////////////////////////////////////////////////////
// Operators
Matrix44& Matrix44::operator *= (const Matrix44 & m2) {
	*this = Matrix44(	m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0],		m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1]  + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1],		m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2]  + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2],		m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3]  + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3],
						m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0],		m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1]  + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1],		m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2]  + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2],		m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3]  + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3],
						m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0],		m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1]  + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1],		m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2]  + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2],		m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3]  + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3],
						m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0],		m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1]  + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1],		m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2]  + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2],		m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3]  + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3] );
	return *this;
};

Matrix44& Matrix44::operator += (const Matrix44 & m2) {
	_11 += m2._11;	_12 += m2._12;	_13 += m2._13;	_14 += m2._14;
	_21 += m2._21;	_22 += m2._22;	_23 += m2._23;	_24 += m2._24;
	_31 += m2._31;	_32 += m2._32;	_33 += m2._33;	_34 += m2._34;
	_41 += m2._41;	_42 += m2._42;	_43 += m2._43;	_44 += m2._44;
	return *this;
}

Matrix44& Matrix44::operator -= (const Matrix44 & m2) {
	_11 -= m2._11;	_12 -= m2._12;	_13 -= m2._13;	_14 -= m2._14;
	_21 -= m2._21;	_22 -= m2._22;	_23 -= m2._23;	_24 -= m2._24;
	_31 -= m2._31;	_32 -= m2._32;	_33 -= m2._33;	_34 -= m2._34;
	_41 -= m2._41;	_42 -= m2._42;	_43 -= m2._43;	_44 -= m2._44;
	return *this;
}

Matrix44& Matrix44::operator *= (const float& f) {
	float* p = (float*)m;
	for (int i=0; i<16; i++) {
		p[i] *= f;
	}
	return *this;
}

Matrix44& Matrix44::operator /= (const float& f) {
	float* p = (float*)m;
	for (int i=0; i<16; i++) {
		p[i] /= f;
	}
	return *this;
}

Matrix44 Matrix44::operator * (const Matrix44 & m2) const {
	return Matrix44(	m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0],		m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1]  + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1],		m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2]  + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2],		m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3]  + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3],
						m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0],		m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1]  + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1],		m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2]  + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2],		m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3]  + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3],
						m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0],		m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1]  + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1],		m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2]  + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2],		m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3]  + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3],
						m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0],		m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1]  + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1],		m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2]  + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2],		m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3]  + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3] );
}

Matrix44 Matrix44::operator + (const Matrix44 & m2) const {
	Matrix44 m = *this;
	m += m2;
	return m;
}

Matrix44 Matrix44::operator - (const Matrix44 & m2) const {
	Matrix44 m = *this;
	m -= m2;
	return m;
}

Matrix44 Matrix44::operator * (const float& f) const {
	Matrix44 m = *this;
	m *= f;
	return m;
}

Matrix44 Matrix44::operator / (const float& f) const {
	Matrix44 m = *this;
	m /= f;
	return m;
}

////////////////////////////////////////////////////////////////////////////////
// Compare
bool Matrix44::operator == (const Matrix44& m2) const {
	return memcmp(this, &m2, sizeof(Matrix44)) == 0;
}

bool Matrix44::operator != (const Matrix44& m2) const {
	return !(*this == m2);
}

////////////////////////////////////////////////////////////////////////////////
// Matrix stuff
Matrix44& Matrix44::Identity() {
	_11 = 1; _12 = 0; _13 = 0; _14 = 0;
	_21 = 0; _22 = 1; _23 = 0; _24 = 0;
	_31 = 0; _32 = 0; _33 = 1; _34 = 0;
	_41 = 0; _42 = 0; _43 = 0; _44 = 1;
	return *this;
}

Matrix44& Matrix44::Inverse() {
	*this = Matrix44Inverse(*this);
	return *this;
}

Matrix44& Matrix44::Transpose() {
	float t;
	t = _12;	_12 = _21;	_21 = t;
	t = _13;	_13 = _31;	_31 = t;
	t = _14;	_14 = _41;	_41 = t;

	t = _24;	_24 = _42;	_42 = t;
	t = _23;	_23 = _32;	_32 = t;
	t = _34;	_34 = _43;	_43 = t;

	return *this;
};

float Matrix44::Determinant() {
	return
		_11 * (_22 * (_33 * _44 - _43 * _34) - _23 * (_32 * _44 - _42 * _34) + _24 * (_32 * _43 - _42 * _33))
		- _12 * (_21 * (_33 * _44 - _43 * _34) - _23 * (_31 * _44 - _41 * _34) + _24 * (_31 * _43 - _41 * _33))
		+ _13 * (_21 * (_32 * _44 - _42 * _34) - _22 * (_31 * _44 - _41 * _34) + _24 * (_31 * _42 - _41 * _32))
		- _14 * (_21 * (_32 * _43 - _42 * _33) - _22 * (_31 * _43 - _41 * _33) + _23 * (_31 * _42 - _32 * _41));
}

// Transformations

// create
Matrix44& Matrix44::RotationX(float angle) {
	_11 = _44 = 1;
	_21 = _24 = 0;
	_31 = _34 = 0;
	_41 = _42 = _43 = 0;

	_22 = _33 = cos(angle);
	_32 = _23 = sin(angle); _32 *= -1;

	return *this;
}
Matrix44& Matrix44::RotationY(float angle) {
	_12 = _14 = 0;
	_21 = 0; _22 = 1; _23 = _24 = 0;
	_32 = _34 = 0;
	_21 = 0; _22 = _23 = 0; _24 = 1;

	_11 = _33 = cos(angle);
	_13 = _31 = sin(angle); _13 *= -1;

	return *this;
}
Matrix44& Matrix44::RotationZ(float angle) {
	_13 = _14 = 0;
	_23 = _24 = 0;
	_31 = _32 = _34 = 0;
	_41 = _42 = _43 = 0;
	_33 = _44 = 1;

	_11 = _22 = cos(angle);
	_21 = _12 = sin(angle); _21 *= -1;

	return *this;
}

Matrix44& Matrix44::RotationQuat(const Quat& q) {
	_11 = 1.f - 2.f*(q.y*q.y + q.z*q.z);	_12 = 2.f*(q.x*q.y + q.z*q.w);			_13 = 2.f*(q.x*q.z - q.y*q.w);			_14 = 0.f;
	_21 = 2.f*(q.x*q.y - q.z*q.w);			_22 = 1.f - 2.f*(q.x*q.x + q.z*q.z);	_23 = 2.f*(q.y*q.z + q.x*q.w);			_24 = 0.f;
	_31 = 2.f*(q.x*q.z + q.y*q.w);			_32 = 2.f*(q.y*q.z - q.x*q.w);			_33 = 1.f - 2.f*(q.x*q.x + q.y*q.y);	_34 = 0.f;
	_41 = 0.f;								_42 = 0.f;								_43 = 0.f;								_44 = 1.f;

	return *this;
}
Matrix44& Matrix44::RotationEuler(const Vec3& rot) {
	RotationX(rot.x);
	*this *= Matrix44RotationY(rot.y);
	*this *= Matrix44RotationY(rot.z);

	return *this;
}
Matrix44& Matrix44::RotationAxisAngle(const Vec3& axis, float angle) {
	float c = cos(angle);
	float s = sin(angle);
	float t = 1.f - c;

	_11 = t*axis.x*axis.x + c;			_12 = t*axis.x*axis.y + axis.z*s;	_13 = t*axis.x*axis.z - axis.y*s;
	_21 = t*axis.x*axis.y - axis.z*s;	_22 = t*axis.y*axis.y + c;			_23 = t*axis.y*axis.z + axis.x*s;
	_31 = t*axis.x*axis.z + axis.y*s;	_32 = t*axis.y*axis.z - axis.x*s;	_33 = t*axis.z*axis.z + c;

	_41 = 0.f;
	_42 = 0.f;
	_43 = 0.f;
	_14 = 0.f;
	_24 = 0.f;
	_34 = 0.f;
	_44 = 1.f;

	return *this;
}

Matrix44& Matrix44::Translation(const Vec3& v) {
	_12 = _13 = _14 = 0.0f;
	_21 = _23 = _24 = 0.0f;
	_31 = _32 = _34 = 0.0f;

	_11 = _22 = _33 = _44 = 1.0f;

	_41 = v.x;
	_42 = v.y;
	_43 = v.z;

	return *this;
}
Matrix44& Matrix44::Scaling(const Vec3& s) {
	_12 = _13 = _14 = 0.0f;
	_21 = _23 = _24 = 0.0f;
	_31 = _32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;

	_11 = s.x;
	_22 = s.y;
	_33 = s.z;
	_44 = 1.0f;

	return *this;
}

Matrix44& Matrix44::ViewRH(const Vec3& eye, const Vec3& target, const Vec3& up) {
	*this = Matrix44ViewRH(eye, target, up);

	return *this;
}
Matrix44& Matrix44::ProjPerspectiveRH(float nearPlane, float farPlane, float fovRad, float aspectRatio) 
{
	_12 = _13 = _14 = 0;
	_21 = _23 = _24 = 0;
	_31 = _32 = 0;
	_41 = _42 = 0;

	_11 = _22 = 1.0f / tanf(fovRad * 0.5f);
	_11 /= aspectRatio;
	_33 = farPlane / (nearPlane - farPlane);
	_43 = (farPlane * nearPlane) / (nearPlane - farPlane);
	_34 = -1;
	_44 = 0;

	return *this;
}

Matrix44& Matrix44::ProjPerspectiveLH(float nearPlane, float farPlane, float fovRad, float aspectRatio) 
{
	_12 = _13 = _14 = 0;
	_21 = _23 = _24 = 0;
	_31 = _32 = 0;
	_41 = _42 = 0;

	_11 = _22 = 1.0f / tanf(fovRad * 0.5f);
	_11 /= aspectRatio;
	_33 = farPlane / (farPlane - nearPlane);
	_43 = (farPlane * nearPlane) / (nearPlane - farPlane);
	_34 = 1;
	_44 = 0;

	return *this;
}

Matrix44& Matrix44::ProjOrtographic(float nearPlane, float farPlane, float left, float right, float bottom, float top) {
	Vec3 center = {left + right, top + bottom, nearPlane + farPlane};
	Vec3 dim = {right - left, top - bottom, farPlane - nearPlane};
	center /= 2.0f;
	
	Translation(-center);
	*this *= Matrix44Scaling(Vec3(1, -1, 0.5) / dim);
	*this *= Matrix44Translation(Vec3(0, 0, 0.5));

	return *this;
}

// pre-concat
Matrix44& Matrix44::PreRotate(const Quat& q) {
	return *this = Matrix44RotationQuat(q) * *this;
}

Matrix44& Matrix44::PreScale(float x, float y, float z) {
	return *this = Matrix44Scaling(x, y, z) * *this;
}

Matrix44& Matrix44::PreScale(const Vec3& s) {
	return *this = Matrix44Scaling(s) * *this;
}

Matrix44& Matrix44::PreTranslate(float x, float y, float z) {
	return *this = Matrix44Translation(x, y, z) * *this;
}

Matrix44& Matrix44::PreTranslate(const Vec3& t) {
	return *this = Matrix44Translation(t) * *this;
}

// post-concat
Matrix44& Matrix44::PostRotate(const Quat& q) {
	return *this *= Matrix44RotationQuat(q);
}

Matrix44& Matrix44::PostScale(float x, float y, float z) {
	return *this *= Matrix44Scaling(x, y, z);
}

Matrix44& Matrix44::PostScale(const Vec3& s) {
	return *this *= Matrix44Scaling(s);
}

Matrix44& Matrix44::PostTranslate(float x, float y, float z) {
	return *this *= Matrix44Translation(x,y,z);
}

Matrix44& Matrix44::PostTranslate(const Vec3& t) {
	return *this *= Matrix44Translation(t);
}


///////////////////////////////////////////////////////////////////////////
// utility functions
void MatrixPrint (const Matrix44& m, std::ostream& os/*=std::cout*/) {
	for (unsigned row = 0; row<4; row++) {
		for (unsigned col = 0; col<4; col++) {
			os << m(row,col) << "	";
		}
		os << "\n";
	}
}

std::ostream& operator << (std::ostream& os, const Matrix44& m) {
	MatrixPrint(m, os);
	return os;
}



//------------------------------------GLOBALS-------------------------------------//
Matrix44 lerp(const Matrix44& m1, const Matrix44& m2, float t) {
	return m2 * t + m1 * (1 - t);
}


Matrix44 Matrix44Transpose(const Matrix44& in) {
	Matrix44 out = in;
	out.Transpose();
	return out;
}

Matrix44 Matrix44Inverse(const Matrix44& in) {
	Matrix44 out;
	// determinant
	float det_A;
	// partial computations for speed optimizations (hopefully faster, should be tested)
	float
		A = in._33 * in._44 - in._43 * in._34,
		B = in._32 * in._44 - in._42 * in._34,
		C = in._32 * in._43 - in._42 * in._33,
		D = in._31 * in._44 - in._41 * in._34,
		E = in._31 * in._43 - in._41 * in._33,
		F = in._31 * in._42 - in._41 * in._32,


		G = in._23 * in._44 - in._43 * in._24,
		H = in._22 * in._44 - in._42 * in._24,
		I = in._22 * in._43 - in._42 * in._23,
		J = in._21 * in._44 - in._41 * in._24,
		K = in._21 * in._43 - in._41 * in._23,
		L = in._21 * in._42 - in._41 * in._22,


		M = in._23 * in._34 - in._33 * in._24,
		N = in._22 * in._34 - in._32 * in._24,
		O = in._22 * in._33 - in._32 * in._23,
		P = in._21 * in._34 - in._31 * in._24,
		Q = in._21 * in._33 - in._31 * in._23,
		R = in._21 * in._32 - in._31 * in._22;

	// A; B; C; D; E; F
	out._11 = (in._22*A - in._23*B + in._24*C);
	out._21 = -(in._21*A - in._23*D + in._24*E);
	out._31 = (in._21*B - in._22*D + in._24*F);
	out._41 = -(in._21*C - in._22*E + in._23*F);

	det_A = 1.0f / (in._11*out._11 + in._12 * out._21 + in._13 * out._31 + in._14 * out._41);

	out._12 = -(in._12*A - in._13*B + in._14*C) * det_A;
	out._22 = (in._11*A - in._13*D + in._14*E) * det_A;
	out._32 = -(in._11*B - in._12*D + in._14*F) * det_A;
	out._42 = (in._11*C - in._12*E + in._13*F) * det_A;

	// G, H, I, J, K, L
	out._13 = (in._12*G - in._13*H + in._14*I) * det_A;
	out._23 = -(in._11*G - in._13*J + in._14*K) * det_A;
	out._33 = (in._11*H - in._12*J + in._14*L) * det_A;
	out._43 = -(in._11*I - in._12*K + in._13*L) * det_A;

	// M, N, O, P, Q, R
	out._14 = -(in._12*M - in._13*N + in._14*O) * det_A;
	out._24 = (in._11*M - in._13*P + in._14*Q) * det_A;
	out._34 = -(in._11*N - in._12*P + in._14*R) * det_A;
	out._44 = (in._11*O - in._12*Q + in._13*R) * det_A;

	// post multiplication by det_A, cause det_a was not available when these were computed
	out._11 *= det_A;
	out._21 *= det_A;
	out._31 *= det_A;
	out._41 *= det_A;

	return out;
}


Matrix44 Matrix44Translation(float x, float y, float z) {
	Matrix44 m;
	m._41 = x;
	m._42 = y;
	m._43 = z;
	return m;
}

Matrix44 Matrix44Translation(const Vec3& v) {
	Matrix44 m;
	m._41 = v.x;
	m._42 = v.y;
	m._43 = v.z;
	return m;
}

Matrix44 Matrix44Scaling(float x, float y, float z) {
	return Matrix44(x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);
}

Matrix44 Matrix44Scaling(const Vec3& v) {
	return Matrix44(v.x, 0, 0, 0,
		0, v.y, 0, 0,
		0, 0, v.z, 0,
		0, 0, 0, 1);
}

Matrix44 Matrix44RotationEuler(const Vec3& rot) {
	return Matrix44RotationX(rot.x) * Matrix44RotationY(rot.y) * Matrix44RotationZ(rot.z);
}

Matrix44 Matrix44RotationEuler(float x, float y, float z) {
	return Matrix44RotationX(x) * Matrix44RotationY(y) * Matrix44RotationZ(z);
}

Matrix44 Matrix44RotationX(float angle) {
	Matrix44 m;
	m._22 = m._33 = cos(angle);
	m._32 = m._23 = sin(angle); m._32 *= -1;
	return m;
}

Matrix44 Matrix44RotationY(float angle) {
	Matrix44 m;
	m._11 = m._33 = cos(angle);
	m._13 = m._31 = sin(angle); m._13 *= -1;
	return m;
}

Matrix44 Matrix44RotationZ(float angle) {
	Matrix44 m;
	m._11 = m._22 = cos(angle);
	m._21 = m._12 = sin(angle); m._21 *= -1;
	return m;
}

Matrix44 Matrix44RotationAxisAngle(const Vec3& axis, float angle) {
	Matrix44 m;
	m.RotationAxisAngle(axis, angle);
	return m;
};

Matrix44 Matrix44RotationQuat(const Quat& q) {
	Matrix44 m;
	m.RotationQuat(q);
	return m;
};


Matrix44 Matrix44ViewRH(const Vec3& eye, const Vec3& target, const Vec3& up) {
	Vec3 baseFront = (target - eye).Normalize();		// The "look-at" vector.
	Vec3 baseRight = Cross(baseFront, up).Normalize();	// The "right" vector.
	Vec3 baseUp = Cross(baseRight, baseFront);			// The "up" vector.

	// Create a 4x4 orientation matrix from the right, up, and at vectors
	// TRANPOSE of ROT
	Matrix44 orientation(	baseRight.x, baseUp.x, baseFront.x, 0,
							baseRight.y, baseUp.y, baseFront.y, 0,
							baseRight.z, baseUp.z, baseFront.z, 0,
							0, 0, 0, 1);

	// Create a 4x4 translation matrix by negating the eye position.
	// NEGATE POS
	Matrix44 translation(	1,		0,		0,		0,
							0,		1,		0,		0,
							0,		0,		1,		0,
							-eye.x, -eye.y, -eye.z, 1);

	// Combine the orientation and translation to compute the view matrix
	return translation * orientation;
}

Matrix44 Matrix44ProjPerspectiveRH(float nearPlane, float farPlane, float fovRad, float aspectRatio) {
	Matrix44 m;
	m.ProjPerspectiveRH(nearPlane, farPlane, fovRad, aspectRatio);
	return m;
}

Matrix44 Matrix44ProjPerspectiveLH(float nearPlane, float farPlane, float fovRad, float aspectRatio) {
	Matrix44 m;
	m.ProjPerspectiveLH(nearPlane, farPlane, fovRad, aspectRatio);
	return m;
}

Matrix44 Matrix44ProjOrtographic(float nearPlane, float farPlane, float left, float right, float bottom, float top) {
	Matrix44 m;
	m.ProjOrtographic(nearPlane, farPlane, left, right, bottom, top);
	return m;
}