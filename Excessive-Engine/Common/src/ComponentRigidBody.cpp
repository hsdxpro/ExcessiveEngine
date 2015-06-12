#include "ComponentRigidBody.h"
#include "..\PhysicsEngine\Interfaces\IEntityRigid.h"

void ComponentRigidBody::UpdateAfterPhysicsSimulate()
{
	WorldComponent::SetTransform(Transform3D(entityRigid->GetPos(), entityRigid->GetRot(), entityRigid->GetScale()));
}

void ComponentRigidBody::SetPos(const mm::vec3& v)
{
	entityRigid->SetPos(v);
	WorldComponent::SetPos(v);
}

void ComponentRigidBody::SetRot(const mm::quat& q)
{
	entityRigid->SetRot(q);
	WorldComponent::SetRot(q);
}

void ComponentRigidBody::SetScale(const mm::vec3& v)
{
	entityRigid->SetScale(v);
	WorldComponent::SetScale(v);
}

const mm::vec3& ComponentRigidBody::GetPos()
{
	return entityRigid->GetPos();
}

const mm::quat& ComponentRigidBody::GetRot()
{
	return entityRigid->GetRot();
}

const mm::vec3& ComponentRigidBody::GetScale()
{
	return entityRigid->GetScale();
}

void ComponentRigidBody::_InnerUpdatePos()
{
	// If mass = 0 maybe
	//entityRigid->SetPos(worldTransform.GetPos());
}

void ComponentRigidBody::_InnerUpdateRot()
{
	// If mass = 0 maybe
	//entityRigid->SetRot(worldTransform.GetRot());
}

void ComponentRigidBody::_InnerUpdateScale()
{
	// If mass = 0 maybe
	//entityRigid->SetScale(worldTransform.GetScale());
}

void ComponentRigidBody::_InnerUpdateSkew()
{

}