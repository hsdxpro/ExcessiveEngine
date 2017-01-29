// Implementation
#include "Vec3.h"
#include "Matrix44.h"

#include "../common.h"

// Convserion to Vec4
Vec3::operator Vec4() {
	return Vec4(*this, 1.0f);
}


////////////////////////////////////////////////////////////////////////////////
// Vector stuff
Vec3& Vec3::Normalize() {
	float scale = 1.0f / (this->Lenght());
	(*this) *= scale;

	return *this;
}

float Vec3::Lenght() const {
	return sqrt(x*x + y*y + z*z);
}

Vec3& Vec3::Cross(const Vec3& v) {
	*this = ::Cross(*this, v);
	return *this;
}

float Vec3::Dot(const Vec3& v) const {
	return x*v.x + y*v.y + z*v.z;
}

////////////////////////////////////////////////////////////////////////////////
// Global vector stuff
Vec3 Normalize(const Vec3& v) {
	Vec3 n = v;
	n.Normalize();
	return n;
}

float Dot(const Vec3& v1, const Vec3& v2) {
	return v1.Dot(v2);
}

Vec3 Cross(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
}

float Lenght(const Vec3& v) {
	return v.Lenght();
}

////////////////////////////////////////////////////////////////////////////////
// Accessors
float Vec3::operator[](size_t idx) const {
	ASSERT(idx >= 0 && idx <= 4);
	return *((float*)this + idx);
}

float& Vec3::operator[](size_t idx) {
	ASSERT(idx >= 0 && idx <= 4);
	return *((float*)this + idx);
}

////////////////////////////////////////////////////////////////////////////////
// Operators
Vec3& Vec3::operator*=(const Vec3& v2) {
	x *= v2.x;
	y *= v2.y;
	z *= v2.z;
	return *this;
}

Vec3& Vec3::operator/=(const Vec3& v2) {
	x /= v2.x;
	y /= v2.y;
	z /= v2.z;
	return *this;
}

Vec3& Vec3::operator += (const Vec3& v2) {
	x += v2.x;
	y += v2.y;
	z += v2.z;
	return *this;
}

Vec3& Vec3::operator -= (const Vec3& v2) {
	x -= v2.x;
	y -= v2.y;
	z -= v2.z;
	return *this;
}

Vec3 Vec3::operator * (const Vec3& v2) const {
	Vec3 vr = *this;
	vr *= v2;
	return vr;
}

Vec3 Vec3::operator / (const Vec3& v2) const {
	Vec3 vr = *this;
	vr /= v2;
	return vr;
}

Vec3 Vec3::operator + (const Vec3& v2) const {
	Vec3 vr = *this;
	vr += v2;
	return vr;
}

Vec3 Vec3::operator - (const Vec3& v2) const {
	Vec3 vr = *this;
	vr -= v2;
	return vr;
}

Vec3 Vec3::operator - () const {
	Vec3 vr(-x, -y, -z);
	return vr;
}

Vec3& Vec3::operator *= (float s) {
	x *= s; y *= s; z *= s;
	return *this;
}

Vec3 Vec3::operator * (float s) const {
	Vec3 vr = *this;
	vr *= s;
	return vr;
}

Vec3 operator*(float s, Vec3 v) {
	return v*s;
}

Vec3& Vec3::operator /= (float s) {
	(*this) *= (1.0f / s);
	return *this;
}

Vec3 Vec3::operator / (float s) const {
	Vec3 vr = *this;
	vr /= s;
	return vr;
}

////////////////////////////////////////////////////////////////////////////////
// Matrix transformation

Vec3& Vec3::operator *= (const Matrix44& m) {
	float tmpX = x;
	float tmpY = y;
	x = m._11 * tmpX + m._21 * tmpY + m._31 * z + m._41;
	y = m._12 * tmpX + m._22 * tmpY + m._32 * z + m._42;
	z = m._13 * tmpX + m._23 * tmpY + m._33 * z + m._43;
	return *this;
}

Vec3 Vec3::operator * (const Matrix44& m) const {
	Vec3 v2 = *this;
	v2 *= m;
	return v2;
}

Vec3 operator*(Matrix44 m, const Vec3& v) {
	m.Transpose();
	return v*m;
}

////////////////////////////////////////////////////////////////////////////////
// Compare
bool Vec3::operator == (const Vec3& w) const {
	if (x == w.x && y == w.y && z == w.z)
		return true;
	else
		return false;
}

bool Vec3::operator != (const Vec3& w) const {
	return !(*this == w);
}

////////////////////////////////////////////////////////////////////////////////
// Cout vectors
std::ostream& operator << (std::ostream& os, Vec3 v) {
	os << v.x << ',' << v.y << ',' << v.z;
	return os;
}
