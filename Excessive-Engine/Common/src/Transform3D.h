#pragma once
#include "mymath\mymath.h"

class Transform3D
{
public:
	Transform3D();
	Transform3D(const mm::vec3& pos, const mm::quat& rot, const mm::vec3& scale);

	const Transform3D inverse() const;
	const mm::mat4 matrix() const;

	_inline void SetPos(const mm::vec3& v) { pos = v; }
	_inline void SetRot(const mm::quat& q) { rot = q; }
	_inline void SetScale(const mm::vec3& v) { scale = v; }
	_inline void SetSkew(const mm::mat3& m) { skew = m; }

	_inline void Move(const mm::vec3& v) { SetPos(pos + v); }
	_inline void Rot(const mm::quat& q) { SetRot(rot * q); } // TODO bad quat multip
	_inline void Scale(const mm::vec3& v) { SetScale(scale * v); }

	__inline const mm::vec3& GetPos() const { return pos; }
	__inline const mm::quat& GetRot() const { return rot; }
	__inline const mm::vec3& GetScale() const { return scale; }
	__inline const mm::mat3& GetSkew() const { return skew; }

	const Transform3D operator * (const Transform3D& other) const;

protected:
	mm::vec3 pos;
	mm::quat rot;
	mm::vec3 scale;
	mm::mat3 skew;
};
