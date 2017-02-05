#pragma once
#include "mymath\mymath.h"

class Transform3D
{
public:
	inline Transform3D();
	inline Transform3D(const mm::vec3& pos, const mm::quat& rot, const mm::vec3& scale);
	inline Transform3D(const Transform3D& from, const Transform3D& to);

	inline void SetPos(const mm::vec3& v);
	inline void SetRot(const mm::quat& q);
	inline void SetRot(const mm::quat& q, const mm::vec3& rotOrigin);
	inline void SetScale(const mm::vec3& v);
	inline void SetScale(const mm::vec3& scale, const mm::vec3& rootPos, const mm::quat& rootRot);
	inline void SetSkew(const mm::mat3& m);

	inline  void Move(const mm::vec3& v);
	inline  void Rot(const mm::quat& q);
	inline  void Rot(const mm::quat& q, const mm::vec3& rotOrigin);
	inline  void Scale(const mm::vec3& scale, const mm::vec3& rootPos, const mm::quat& rootRot);
	inline  void Scale(const mm::vec3& scale);

	inline  const mm::vec3& GetPos() const;
	inline  const mm::quat& GetRot() const;
	inline  const mm::mat3& GetSkew() const;
	inline  const mm::vec3  GetScale() const;

	inline  mm::vec3 GetFrontDir()	const;
	inline  mm::vec3 GetBackDir()	const;
	inline  mm::vec3 GetUpDir()		const;
	inline  mm::vec3 GetDownDir()	const;
	inline  mm::vec3 GetRightDir()	const;
	inline  mm::vec3 GetLeftDir()	const;

	inline  Transform3D operator * (const Transform3D& b);

protected:
	mm::vec3 pos;
	mm::quat rot;
	mm::mat3 skew;
};

Transform3D::Transform3D()
:pos(0, 0, 0), rot(0, { 0,0,1 })
{
	skew[0] = { 1, 0, 0 };
	skew[1] = { 0, 1, 0 };
	skew[2] = { 0, 0, 1 };
}

Transform3D::Transform3D(const mm::vec3& pos, const mm::quat& rot, const mm::vec3& scale)
:pos(pos), rot(rot)
{
	skew[0] = { scale.x, 0, 0 };
	skew[1] = { 0, scale.y, 0 };
	skew[2] = { 0, 0, scale.z };
}

Transform3D::Transform3D(const Transform3D& from, const Transform3D& to)
{
	SetRot(to.GetRot() * mm::inverse(from.GetRot()));
	//SetScale(to.GetScale() / from.GetScale());
	SetPos(mm::rotate_vector(mm::inverse(from.GetRot()), (to.GetPos() - from.GetPos()) / from.GetScale()));
	SetSkew(to.GetSkew() * mm::inverse(from.GetSkew()));
}

void Transform3D::SetPos(const mm::vec3& v)
{
	pos = v;
}

void Transform3D::SetRot(const mm::quat& q)
{
	rot = q;
}

void Transform3D::SetRot(const mm::quat& q, const mm::vec3& rotOrigin)
{
	pos = rotOrigin + mm::rotate_vector(q * mm::inverse(rot), pos - rotOrigin);
	rot = q;
}

void Transform3D::Rot(const mm::quat& q, const mm::vec3& rotOrigin)
{
	pos = rotOrigin + mm::rotate_vector(q, pos - rotOrigin);
	rot = q * rot;
}

void Transform3D::SetScale(const mm::vec3& v)
{
	skew *= mm::create_scale(v);
}

void Transform3D::SetScale(const mm::vec3& scale, const mm::vec3& rootPos, const mm::quat& rootRot)
{
	mm::vec3 dScale = scale / GetScale();
	Scale(dScale, rootPos, rootRot);
}

void Transform3D::Scale(const mm::vec3& scale, const mm::vec3& rootPos, const mm::quat& rootRot)
{
	mm::mat3 dRot = mm::mat3(rot * mm::inverse(rootRot));

	pos = rootPos + mm::rotate_vector(rootRot, mm::rotate_vector(mm::inverse(rootRot), pos - rootPos) * scale);
	skew = skew * mm::inverse(dRot) * mm::mat3(mm::create_scale(scale)) * dRot;
}

void Transform3D::Scale(const mm::vec3& scale)
{
	skew = skew * mm::mat3(mm::create_scale(scale));
}

void Transform3D::SetSkew(const mm::mat3& m)
{
	skew = m;
}

void Transform3D::Move(const mm::vec3& v)
{
	SetPos(pos + v);
}

void Transform3D::Rot(const mm::quat& q)
{
	SetRot(rot * q);
}

const mm::vec3& Transform3D::GetPos() const
{
	return pos;
}

const mm::quat& Transform3D::GetRot() const
{
	return rot;
}

const mm::mat3& Transform3D::GetSkew() const
{
	return skew;
}

const mm::vec3 Transform3D::GetScale() const
{
	return skew * mm::vec3(1, 1, 1);
}

mm::vec3 Transform3D::GetFrontDir() const
{
	return mm::normalize(mm::rotate_vector(GetRot(), mm::vec3(0, 0, 1) * GetSkew()));
}

mm::vec3 Transform3D::GetBackDir() const
{
	return mm::normalize(mm::rotate_vector(GetRot(), mm::vec3(0, 0, -1) * GetSkew()));
}

mm::vec3 Transform3D::GetUpDir() const
{
	return mm::normalize(mm::rotate_vector(GetRot(), mm::vec3(0, 1, 0) * GetSkew()));
}

mm::vec3 Transform3D::GetDownDir() const
{
	return mm::normalize(mm::rotate_vector(GetRot(), mm::vec3(0, -1, 0) * GetSkew()));
}

mm::vec3 Transform3D::GetRightDir() const
{
	return mm::normalize(mm::rotate_vector(GetRot(), mm::vec3(1, 0, 0) * GetSkew()));
}

mm::vec3 Transform3D::GetLeftDir() const
{
	return mm::normalize(mm::rotate_vector(GetRot(), mm::vec3(-1, 0, 0) * GetSkew()));
}

Transform3D Transform3D::operator * (const Transform3D& b)
{
	// TODO
	Transform3D result;
	return result;
}
