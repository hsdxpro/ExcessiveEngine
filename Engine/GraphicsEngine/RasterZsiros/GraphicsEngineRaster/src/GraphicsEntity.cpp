////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Scene entity for Graphics Engine
//		implementation
////////////////////////////////////////////////////////////////////////////////


#include "GraphicsEntity.h"


cGraphicsEntity::cGraphicsEntity()
:isVisible(true), instanceGroup(nullptr) {
}

void cGraphicsEntity::SetPos(const Vec3& p) {
	SetPos(p.x, p.y, p.z);
}
void cGraphicsEntity::SetPos(float x, float y, float z) {
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void cGraphicsEntity::SetRot(const Quat& r) {
	SetRot(r.x, r.y, r.z, r.w);
}
void cGraphicsEntity::SetRot(float x, float y, float z, float w) {
	rot.x = x;
	rot.y = y;
	rot.z = z;
	rot.w = w;
}

void cGraphicsEntity::SetScale(const Vec3& s) {
	SetScale(s.x, s.y, s.z);
}

void cGraphicsEntity::SetScale(float x, float y, float z) {
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

const Vec3& cGraphicsEntity::GetPos() const {
	return pos;
}

const Quat& cGraphicsEntity::GetRot() const {
	return rot;
}

const Vec3& cGraphicsEntity::GetScale() const {
	return scale;
}