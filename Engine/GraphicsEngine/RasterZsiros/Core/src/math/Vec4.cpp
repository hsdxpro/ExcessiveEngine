#include "Vec4.h"
#include "vec3.h"
#include "Matrix44.h"

// Convert to vec3
Vec4::operator Vec3() {
	return Vec3(x, y, z);
}

////////////////////////////////////////////////////////////////////////////////
// Ctors
Vec4::Vec4()
:x(0), y(0), z(0), w(0) {
}

Vec4::Vec4(const Vec3& v, float w)
:x(v.x), y(v.y), z(v.z), w(w) {
}

Vec4::Vec4(float x, float y, float z, float w)
:x(x), y(y), z(z), w(w) {
}

Vec4::Vec4(float v[4])
:x(v[0]), y(v[1]), z(v[2]), w(v[3]) {
}

////////////////////////////////////////////////////////////////////////////////
// Accessors
float Vec4::operator [](size_t idx) const {
	return *(((float*)this) + idx);
}

float& Vec4::operator [](size_t idx) {
	return *(((float*)this) + idx);
}

////////////////////////////////////////////////////////////////////////////////
// Math operators
Vec4& Vec4::operator*=(const Vec4& v2) {
	x *= v2.x;
	y *= v2.y;
	z *= v2.z;
	w *= v2.w;
	return *this;
}

Vec4& Vec4::operator/=(const Vec4& v2) {
	x /= v2.x;
	y /= v2.y;
	z /= v2.z;
	w /= v2.w;
	return *this;
}

Vec4& Vec4::operator += (const Vec4& v2) {
	x += v2.x;
	y += v2.y;
	z += v2.z;
	w += v2.w;
	return *this;
}

Vec4& Vec4::operator -= (const Vec4& v2) {
	x -= v2.x;
	y -= v2.y;
	z -= v2.z;
	w -= v2.w;
	return *this;
}

Vec4 Vec4::operator * (const Vec4& v2) const {
	Vec4 vr = *this;
	vr *= v2;
	return vr;
}

Vec4 Vec4::operator / (const Vec4& v2) const {
	Vec4 vr = *this;
	vr /= v2;
	return vr;
}

Vec4 Vec4::operator + (const Vec4& v2) const {
	Vec4 vr = *this;
	vr += v2;
	return vr;
}

Vec4 Vec4::operator - (const Vec4& v2) const {
	Vec4 vr = *this;
	vr -= v2;
	return vr;
}

Vec4 Vec4::operator - () const {
	Vec4 vr(-x, -y, -z, -w);
	return vr;
}

Vec4& Vec4::operator *= (float s) {
	x*=s; y*=s; z*=s; w*=s;
	return *this;
}

Vec4 Vec4::operator * (float s) const {
	Vec4 vr = *this;
	vr *= s;
	return vr;
}

Vec4& Vec4::operator /= (float s) {
	(*this) *= (1.0f / s);
	return *this;
}

Vec4 Vec4::operator / (float s) const {
	Vec4 vr=*this;
	vr /= s;
	return vr;
}

////////////////////////////////////////////////////////////////////////////////
// Compare
bool Vec4::operator == (const Vec4& v) const {
	return (x == v.x && y == v.y && z == v.z && w == v.w);
}

bool Vec4::operator != (const Vec4& v) const {
	return !(*this == v);
}


////////////////////////////////////////////////////////////////////////////////
// Vector stuff
float Vec4::Lenght() const {
	return sqrt(x*x + y*y + z*z + w*w);
}

Vec4& Vec4::Normalize() {
	float scale = 1.0f / (Lenght());
	(*this) *= scale;

	return *this;
}

float Vec4::Dot(const Vec4& v2) const {
	return x*v2.x + y*v2.y + z*v2.z + w*v2.w;
}

////////////////////////////////////////////////////////////////////////////////
// Global vector stuff
Vec4 Normalize(const Vec4& v) {
	Vec4 n=v;
	n.Normalize();
	return n;
}

float Dot(const Vec4& v1, const Vec4& v2) {
	return v1.Dot(v2);
}

float Lenght(const Vec4& v) {
	return v.Lenght();
}

////////////////////////////////////////////////////////////////////////////////
// Matrix transformations
Vec4 Vec4::operator * (const Matrix44& m) const {
	Vec4 v2 = *this;
	v2 *= m;
	return v2;
};

Vec4& Vec4::operator *= (const Matrix44& m) {
	float tmpX = x, tmpY = y, tmpZ = z;
	x = m._11*tmpX + m._21*tmpY + m._31*tmpZ + m._41*w;
	y = m._12*tmpX + m._22*tmpY + m._32*tmpZ + m._42*w;
	z = m._13*tmpX + m._23*tmpY + m._33*tmpZ + m._43*w;
	w = m._14*tmpX + m._24*tmpY + m._34*tmpZ + m._44*w;
	return *this;
}

Vec4 operator*(Matrix44 m, const Vec4& v) {
	m.Transpose();
	return v*m;
}

////////////////////////////////////////////////////////////////////////////////
// Print to streams
std::ostream& operator<<(std::ostream& os, Vec4 v) {
	os << v.x << ',' << v.y << ',' << v.z;
	return os;
}