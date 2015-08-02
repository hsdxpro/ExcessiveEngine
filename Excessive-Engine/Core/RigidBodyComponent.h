#pragma once
#include "WorldComponent.h"

#include "..\PhysicsEngine\IRigidBodyEntity.h"

class RigidBodyComponent : public WorldComponent
{
public:
	RigidBodyComponent(physics::IRigidBodyEntity* a);

	void UpdateAfterPhysicsSimulate(); // After physics simulation done, we transforms all WorldComponent child also

	__inline void AddForce(const mm::vec3& force, const mm::vec3& relPos = { 0, 0, 0 }) { entity->AddForce(force, relPos); }

	__inline void SetUserPointer(void* p)			{ entity->SetUserPointer(p); }

	__inline void SetGravityScale(float s)			{ entity->SetGravityScale(s); }
	__inline void SetTrigger(bool bTrigger)			{ entity->SetTrigger(bTrigger); }
	__inline void SetCollisionGroup(i64 ID)			{ entity->SetCollisionGroup(ID); }

	__inline void SetAngularFactor(float factor)	{ entity->SetAngularFactor(factor); }
	__inline void SetKinematic(bool bKinematic)		{ entity->SetKinematic(bKinematic); }
	__inline void SetVelocity(const mm::vec3& v)  { entity->SetVelocity(v); }

	__inline i64 GetCollisionGroup() const { return entity->GetCollisionGroup(); }
	__inline mm::vec3 GetVelocity() const { return entity->GetVelocity(); }
	__inline void* GetUserPointer() { return entity->GetUserPointer(); }

	__inline bool IsTrigger() const { return entity->IsTrigger(); }
	__inline bool IsStatic() const { return entity->IsStatic(); }
	__inline bool IsDynamic() const { return entity->IsDynamic(); }

	__inline physics::IRigidBodyEntity* GetEntity() { return entity; }

public:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	physics::IRigidBodyEntity* entity;
};