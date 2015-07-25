#pragma once
#include "mymath\mymath.h"

class Transform3D
{
public:
	Transform3D();
	Transform3D(const mm::vec3& pos, const mm::quat& rot, const mm::vec3& scale);

	_inline void SetPos(const mm::vec3& v) { pos = v; }
	_inline void SetRot(const mm::quat& q) { rot = q; }
	_inline void SetScaleLocal(const mm::vec3& v) { skew[0].x = v.x; skew[1].y = v.y; skew[2].z = v.z; }
	_inline void SetSkew(const mm::mat3& m) { skew = m; }

	_inline void Move(const mm::vec3& v) { SetPos(pos + v); }
	_inline void Rot(const mm::quat& q) { SetRot(rot * q); }

	__inline const mm::vec3& GetPos() const { return pos; }
	__inline const mm::quat& GetRot() const { return rot; }
	__inline const mm::mat3& GetSkew() const { return skew; }
	__inline const mm::vec3  GetScaleLocal() const { return skew * mm::vec3(1,1,1);}

protected:
	mm::vec3 pos;
	mm::quat rot;
	mm::mat3 skew;
};
