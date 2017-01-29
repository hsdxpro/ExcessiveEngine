// Implementation
// history:
// 2012.oct.11, quaternion rotation seems to be perfect

#include "Quat.h"
#include "Matrix44.h"

// ctors
Quat::Quat() 
:x(0.0f), y(0.0f), z(0.0f), w(1.0f) {
}

Quat::Quat(float x, float y, float z, float w) {
	this->x=x;
	this->y=y;
	this->z=z;
	this->w=w;
}

Quat::Quat(const Vec3& v, float angle) {
	float sin_angle = sin(angle*0.5f);
	x = v.x*sin_angle;
	y = v.y*sin_angle;
	z = v.z*sin_angle;
	w = cos(angle*0.5f);
}

// operators
Quat& Quat::operator *= (const Quat& rhs) {
	float a = w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z;
	float b = w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y;
	float c = w*rhs.y - x*rhs.z + y*rhs.w + z*rhs.x;
	float d = w*rhs.z + x*rhs.y - y*rhs.x + z*rhs.w;
	w = a;
	x = b;
	y = c;
	z = d;
	return *this;
}

Quat Quat::operator * (const Quat& rhs) const {
	Quat r = *this;;
	r *= rhs;
	return r;
}

const Quat Quat::operator ~ () const {
	Quat r = *this;
	r.Conjugate();
	return r;
}

bool Quat::operator == (const Quat& r) const {
	if (w==r.w && x==r.x && y==r.y && z==r.z)
		return true;
	else
		return false;
}

bool Quat::operator != (const Quat& r) const {
	return !(*this==r);
}

// quat stuff
float Quat::Lenght() const {
	return sqrt(w*w + x*x + y*y + z*z);
}

Quat& Quat::Normalize() {
	float scale = 1.f/(this->Lenght());
	w*=scale;
	x*=scale;
	y*=scale;
	z*=scale;

	return *this;
}

Quat& Quat::Conjugate() {
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

// globals
Quat Normalize(const Quat& q) {
	Quat n=q;
	n.Normalize();
	return n;
}

float Lenght(const Quat& q) {
	return q.Lenght();
}

float Dot(const Quat& q1, const Quat& q2) {
	return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
}

// interaction with vec3
Vec3 Quat::RotateVec3(Vec3 v, Quat q) {
	Quat qv(v.x, v.y, v.z, 0.f);

	Quat qvr = q*qv*(~q);
	
	Vec3 vr;
	vr.x = qvr.x;
	vr.y = qvr.y;
	vr.z = qvr.z;

	return vr;
}

Vec3 Quat::RotateVec3_2(Vec3 v, Quat q) {
	Vec3 vr;
	float w=q.w, x=q.x, y=q.y, z=q.z;
	vr.x = w*w*v.x + 2*y*w*v.z - 2*z*w*v.y + x*x*v.x + 2*y*x*v.y + 2*z*x*v.z - z*z*v.x - y*y*v.x;
	vr.y = 2*x*y*v.x + y*y*v.y + 2*z*y*v.z + 2*w*z*v.x - z*z*v.y + w*w*v.y - 2*x*w*v.z - x*x*v.y;
	vr.z = 2*x*z*v.x + 2*y*z*v.y + z*z*v.z - 2*w*y*v.x - y*y*v.z + 2*w*x*v.y - x*x*v.z + w*w*v.z;
	return vr;
}

Vec3 operator*(const Vec3& v, const Quat& q) {
	return Quat::RotateVec3_2(v, q);
}

Vec3 Quat::operator*(const Vec3& v) {
	return Quat::RotateVec3_2(v, *this);
}

Vec3& operator*=(Vec3& v, const Quat& q) {
	v = Quat::RotateVec3_2(v, q);
	return v;
}


// rotation properties
Quat Quat::EulerAnglesToQuat(const Vec3& eulerAngles) {
	return EulerAnglesToQuat(eulerAngles.x, eulerAngles.y, eulerAngles.z);
}

Quat Quat::EulerAnglesToQuat(float rotX, float rotY, float rotZ) {
	Quat q;
	float cos_x = cos(rotX * 0.5f);
	float sin_x = sin(rotX * 0.5f);
	float cos_y = cos(rotY * 0.5f);
	float sin_y = sin(rotY * 0.5f);
	float cos_z = cos(rotZ * 0.5f);
	float sin_z = sin(rotZ * 0.5f);
						   
	q.w =	cos_x *	cos_y *	cos_z	+	sin_x *	sin_y *	sin_z;
	q.x =	sin_x *	cos_y *	cos_z	-	cos_x *	sin_y *	sin_z;
	q.y =	cos_x *	sin_y *	cos_z	+	sin_x *	cos_y *	sin_z;
	q.z =	cos_x *	cos_y *	sin_z	-	sin_x *	sin_y *	cos_z;

	return q;
}

Vec3 Quat::QuatToEulerAngles(const Quat& q) {
	Vec3 eulerAngles;
	eulerAngles.x = atan2(2.f*(q.w*q.x+q.y*q.z), 1-2.f*(q.x*q.x+q.y*q.y));
	eulerAngles.y = asin(2.f*(q.w*q.y-q.x*q.z));
	eulerAngles.z = atan2(2.f*(q.w*q.z+q.x*q.y), 1-2.f*(q.y*q.y+q.z*q.z));
	return eulerAngles;
}

Quat Quat::DirToRot(const Vec3&dir, const Vec3& up) {
	// Create basis for the matrix
	Vec3 right = Cross(up, dir);
	Vec3 realUp = Cross(dir, right);

	Matrix44 bMat(	right.x	, right.y	, right.z	, 0,
					realUp.x, realUp.y	, realUp.z	, 0,
					dir.x	, dir.y		, dir.z		, 0,
					0		, 0			, 0			, 1		);

	// Build quat from matrix
	Quat rot;
		rot.w = (float)sqrt(1 + bMat._11 + bMat._22 + bMat._33) / 2;
		float dfWScale = rot.w * 4;
		rot.x = (bMat._32 - bMat._23) / dfWScale;
		rot.y = (bMat._13 - bMat._31) / dfWScale;
		rot.z = (bMat._21 - bMat._12) / dfWScale;
	return rot;
}

// convert to matrix
Quat::operator Matrix44() {
	/*
	Matrix44 m(
		1.f*-2.f*(y*y + z*z), 2.f*(x*y - z*w), 2.f*(x*z + y*w), 0.f,
		2.f*(x*y + z*w), 1.f - 2.f*(x*x + y*y), 2.f*(y*z - x*w), 0.f,
		2.f*(x*z - y*w), 2.f*(y*z + x*w), 1.f - 2.f*(x*x + y*y), 0.f,
		0.f, 0.f, 0.f, 1.f
		);
	*/
	Matrix44 m = Matrix44RotationQuat(*this);
	return m;
};

// cout quaternions
std::ostream& operator<<(std::ostream& os, Quat q) {
	os << q.w << ':' << q.x << ',' << q.y << ',' << q.z;
	return os;
}



