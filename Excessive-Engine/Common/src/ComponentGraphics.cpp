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

void ComponentGraphics::SetScale(const mm::vec3& v)
{
	entityGraphics->SetScale(v);
	WorldComponent::SetScale(v);
}

const mm::vec3& ComponentGraphics::GetPos()
{
	return entityGraphics->GetPos();
}

const mm::quat& ComponentGraphics::GetRot()
{
	return entityGraphics->GetRot();
}

const mm::vec3& ComponentGraphics::GetScale()
{
	return entityGraphics->GetScale();
}

void ComponentGraphics::_InnerUpdatePos()
{
	entityGraphics->SetPos(worldTransform.GetPos());
}

void ComponentGraphics::_InnerUpdateRot()
{
	entityGraphics->SetRot(worldTransform.GetRot());
}

void ComponentGraphics::_InnerUpdateScale()
{
	//entityGraphics->SetScale(worldTransform.GetScale());
}

void ComponentGraphics::_InnerUpdateSkew()
{
	entityGraphics->SetSkew(worldTransform.GetSkew());
}