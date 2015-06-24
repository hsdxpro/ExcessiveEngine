#include "ComponentGraphics.h"
#include "..\GraphicsEngine\Interfaces\IEntity.h"

void ComponentGraphics::SetPos(const mm::vec3& v)
{
	entityGraphics->SetPos(v);
	WorldComponent::SetPos(v);
}

void ComponentGraphics::SetRot(const mm::quat& q)
{
	entityGraphics->SetRot(q);
	WorldComponent::SetRot(q);
}

void ComponentGraphics::SetScaleLocal(const mm::vec3& v)
{
	entityGraphics->SetScaleLocal(v);
	WorldComponent::SetScaleLocal(v);
}

const mm::vec3& ComponentGraphics::GetPos()
{
	return entityGraphics->GetPos();
}

const mm::quat& ComponentGraphics::GetRot()
{
	return entityGraphics->GetRot();
}

const mm::vec3& ComponentGraphics::GetScaleLocal()
{
	return entityGraphics->GetScaleLocal();
}

void ComponentGraphics::_InnerReflectPos()
{
	entityGraphics->SetPos(worldTransform.GetPos());
}

void ComponentGraphics::_InnerReflectRot()
{
	entityGraphics->SetRot(worldTransform.GetRot());
}

void ComponentGraphics::_InnerReflectSkew()
{
	entityGraphics->SetSkew(worldTransform.GetSkew());
}