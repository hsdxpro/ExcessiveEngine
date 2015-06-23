#include "ComponentRigidBody.h"
#include "..\PhysicsEngine\Interfaces\IEntityRigid.h"

void ComponentRigidBody::UpdateAfterPhysicsSimulate()
{
	WorldComponent::SetPos(entityRigid->GetPos());
	WorldComponent::SetRot(entityRigid->GetRot());
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

void ComponentRigidBody::SetScaleLocal(const mm::vec3& v)
{
	entityRigid->SetScaleLocal(v);
	WorldComponent::SetScaleLocal(v);
}

const mm::vec3& ComponentRigidBody::GetPos()
{
	return entityRigid->GetPos();
}

const mm::quat& ComponentRigidBody::GetRot()
{
	return entityRigid->GetRot();
}

const mm::vec3& ComponentRigidBody::GetScaleLocal()
{
	return entityRigid->GetScaleLocal();
}

void ComponentRigidBody::_InnerReflectPos()
{
	// If mass = 0 maybe
	//entityRigid->SetPos(worldTransform.GetPos());
}

void ComponentRigidBody::_InnerReflectRot()
{
	// If mass = 0 maybe
	//entityRigid->SetRot(worldTransform.GetRot());
}

void ComponentRigidBody::_InnerReflectSkew()
{

}