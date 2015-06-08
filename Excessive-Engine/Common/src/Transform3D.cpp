#include "Transform3D.h"

Transform3D::Transform3D()
:pos_(0, 0, 0), scale_(1,1,1)
{

}

Transform3D::Transform3D(const mm::vec3& pos, const mm::quat& rot, const mm::vec3& scale)
:pos_(pos), rot_(rot), scale_(scale)
{

}

const Transform3D Transform3D::inverse() const
{
	Transform3D inv;
		inv.setPos(-getPos());
		inv.setRot(mm::inverse(getRot()));

		// non 100% percent safety, more speed in release, dont use transform3D when we speak about 3d point
		assert(scale_.x * scale_.x + scale_.y * scale_.y != 0);

		inv.setScale(mm::vec3(1, 1, 1) / scale_);

	return inv;
}

const mm::mat4 Transform3D::matrix() const
{
	return mm::create_translation(getPos()) * mm::mat4(getRot()) * mm::create_scale(getScale());
}

const Transform3D Transform3D::operator * (const Transform3D& other) const
{
	Transform3D result;
		result.rot_ = rot_ * other.rot_; // TODO
		result.scale_ = scale_ * other.scale_;
		result.pos_ = mm::rotate_vector(other.rot_, (other.scale_ * pos_)) + other.pos_;
	return result;
}