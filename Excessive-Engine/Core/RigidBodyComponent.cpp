#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(physics::IRigidEntity* a)
:RigidEntity(a)
{
}

void RigidBodyComponent::UpdateAfterPhysicsSimulate()
{
	WorldComponent::SetPos(RigidEntity->GetPos());
	WorldComponent::SetRot(RigidEntity->GetRot());
}

void RigidBodyComponent::_InnerReflectPos()
{
	RigidEntity->SetPos(transform.GetPos());
}

void RigidBodyComponent::_InnerReflectRot()
{
	RigidEntity->SetRot(transform.GetRot());
}

void RigidBodyComponent::_InnerReflectSkew()
{
	assert(0);
}
