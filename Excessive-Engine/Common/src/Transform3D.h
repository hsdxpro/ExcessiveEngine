#pragma once
#include "mymath\mymath.h"

class Transform3D
{
public:
	Transform3D();
	Transform3D(const mm::vec3& pos, const mm::quat& rot, const mm::vec3& scale);

	const Transform3D inverse() const;
	const mm::mat4 matrix() const;

	_inline void setPos(const mm::vec3& v) { pos_ = v; }
	_inline void setRot(const mm::quat& q) { rot_ = q; }
	_inline void setScale(const mm::vec3& v) { scale_ = v; }

	_inline void move(const mm::vec3& v) { setPos(pos_ + v); }
	_inline void rot(const mm::quat& q) { setRot(rot_ * q); } // TODO bad quat multip
	_inline void scale(const mm::vec3& v) { setScale(scale_ * v); }

	__inline const mm::vec3& getPos() const { return pos_; }
	__inline const mm::quat& getRot() const { return rot_; }
	__inline const mm::vec3& getScale() const { return scale_; }

	const Transform3D operator * (const Transform3D& other) const;

protected:
	mm::vec3 pos_;
	mm::quat rot_;
	mm::vec3 scale_;
};
