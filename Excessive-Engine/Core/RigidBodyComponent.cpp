#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(physics::IRigidBodyEntity* a)
:entity(a)
{
}

void RigidBodyComponent::UpdateAfterPhysicsSimulate()
{
	WorldComponent::SetPos(entity->GetPos());
	WorldComponent::SetRot(entity->GetRot());
}

void RigidBodyComponent::_InnerReflectPos()
{
	entity->SetPos(transform.GetPos());
}

void RigidBodyComponent::_InnerReflectRot()
{
	entity->SetRot(transform.GetRot());
}

void RigidBodyComponent::_InnerReflectSkew()
{
	entity->SetScaleLocal(transform.GetScaleLocal());
}
