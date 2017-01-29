#pragma once
#include "WorldComponent.h"

#include "PhysicsEngine/IRigidBodyEntity.h"

class RigidBodyComponent : public WorldComponent
{
public:
	static const eWorldComponentType TYPE = RIGID_BODY;

public:
	inline RigidBodyComponent(physics::IRigidBodyEntity* a);

	inline void AddForce(const mm::vec3& force, const mm::vec3& relPos = { 0, 0, 0 });

	inline void SetUserPointer(void* p);

	inline void SetGravityScale(float s);
	inline void SetTrigger(bool bTrigger);
	inline void SetCollisionGroup(u64 ID);

	inline void SetAngularFactor(float factor);
	inline void SetKinematic(bool bKinematic);
	inline void SetVelocity(const mm::vec3& v);

	inline u64 GetCollisionGroup() const;
	inline mm::vec3 GetVelocity() const;
	inline void* GetUserPointer();

	inline bool IsTrigger() const;
	inline bool IsStatic() const;
	inline bool IsDynamic() const;
	inline bool IsKinematic() const;

	inline physics::IRigidBodyEntity* GetEntity();

	inline std::vector<physics::ContactPoint> GetContactPoints() const;

protected:
	physics::IRigidBodyEntity* entity;
};




RigidBodyComponent::RigidBodyComponent(physics::IRigidBodyEntity* a)
:WorldComponent(TYPE), entity(a)
{

}

void RigidBodyComponent::AddForce(const mm::vec3& force, const mm::vec3& relPos /*= { 0, 0, 0 }*/)
{
	entity->AddForce(force, relPos);
}

void RigidBodyComponent::SetUserPointer(void* p)
{
	entity->SetUserPointer(p);
}

void RigidBodyComponent::SetGravityScale(float s)
{
	entity->SetGravityScale(s);
}

void RigidBodyComponent::SetTrigger(bool bTrigger)
{
	entity->SetTrigger(bTrigger);
}

void RigidBodyComponent::SetCollisionGroup(u64 ID)
{
	entity->SetCollisionGroup(ID);
}

void RigidBodyComponent::SetAngularFactor(float factor)
{
	entity->SetAngularFactor(factor);
}

void RigidBodyComponent::SetKinematic(bool bKinematic)
{
	entity->SetKinematic(bKinematic);
}

void RigidBodyComponent::SetVelocity(const mm::vec3& v)
{
	entity->SetVelocity(v);
}

u64 RigidBodyComponent::GetCollisionGroup() const
{
	return entity->GetCollisionGroup();
}

mm::vec3 RigidBodyComponent::GetVelocity() const
{
	return entity->GetVelocity();
}

void* RigidBodyComponent::GetUserPointer()
{
	return entity->GetUserPointer();
}

bool RigidBodyComponent::IsTrigger() const
{
	return entity->IsTrigger();
}

bool RigidBodyComponent::IsStatic() const
{
	return entity->IsStatic();
}

bool RigidBodyComponent::IsDynamic() const
{
	return entity->IsDynamic();
}

bool RigidBodyComponent::IsKinematic() const
{
	return entity->IsKinematic();
}

physics::IRigidBodyEntity* RigidBodyComponent::GetEntity()
{
	return entity;
}

std::vector<physics::ContactPoint> RigidBodyComponent::GetContactPoints() const
{
	return entity->GetContactPoints();
}