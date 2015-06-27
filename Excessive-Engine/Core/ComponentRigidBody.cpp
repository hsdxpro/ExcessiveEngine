#include "ComponentRigidBody.h"
#include "..\PhysicsEngine\Interfaces\IEntityRigid.h"
#include "..\GraphicsEngine\Interfaces\IEntity.h"

ComponentRigidBody::ComponentRigidBody(physics::IEntityRigid* a)
:entityRigid(a)
{
}

void ComponentRigidBody::UpdateAfterPhysicsSimulate()
{
	WorldComponent::SetPos(entityRigid->GetPos());
	WorldComponent::SetRot(entityRigid->GetRot());
}

void ComponentRigidBody::_InnerReflectPos()
{
	entityRigid->SetPos(transform.GetPos());
}

void ComponentRigidBody::_InnerReflectRot()
{
	entityRigid->SetRot(transform.GetRot());
}

void ComponentRigidBody::_InnerReflectSkew()
{
	assert(0);
}
