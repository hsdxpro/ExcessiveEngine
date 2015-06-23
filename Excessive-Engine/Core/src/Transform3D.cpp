#include "Transform3D.h"

Transform3D::Transform3D()
:pos(0, 0, 0)
{
	skew[0] = {1, 0, 0};
	skew[1] = {0, 1, 0};
	skew[2] = {0, 0, 1};
}

Transform3D::Transform3D(const mm::vec3& pos, const mm::quat& rot, const mm::vec3& scale)
:pos(pos), rot(rot)
{
	skew[0] = {scale.x, 0, 0};
	skew[1] = {0, scale.y, 0};
	skew[2] = {0, 0, scale.z};
}

//const Transform3D Transform3D::inverse() const
//{
//	Transform3D inv;
//		inv.SetPos(-GetPos());
//		inv.SetRot(mm::inverse(GetRot()));
//
//		// non 100% percent safety, more speed in release, dont use transform3D when we speak about 3d point
//		assert(scale.x * scale.x + scale.y * scale.y != 0);
//
//		inv.SetScale(mm::vec3(1, 1, 1) / scale);
//
//	return inv;
//}

//const mm::mat4 Transform3D::matrix() const
//{
//	return mm::create_translation(GetPos()) * mm::mat4(GetRot()) * mm::create_scale(GetScale());
//}

//const Transform3D Transform3D::operator * (const Transform3D& other) const
//{
//	Transform3D result;
//		result.rot = rot * other.rot;
//		result.scale = scale * other.scale;
//		result.pos = mm::rotate_vector(other.rot, (other.scale * pos)) + other.pos;
//	return result;
//}