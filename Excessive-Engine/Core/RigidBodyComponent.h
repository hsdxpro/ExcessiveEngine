#pragma once
#include "WorldComponent.h"

#include "..\PhysicsEngine\IRigidEntity.h"

class RigidBodyComponent : public WorldComponent
{
public:
	RigidBodyComponent(physics::IRigidEntity* a);

	void UpdateAfterPhysicsSimulate(); // After physics simulation done, we transforms all WorldComponent child also

	__inline void AddForce(const mm::vec3& force, const mm::vec3& relPos = {0,0,0}) {RigidEntity->AddForce(force, relPos);}

	__inline void SetAngularFactor(float factor)	{RigidEntity->SetAngularFactor(factor);}
	__inline void SetKinematic()					{RigidEntity->SetKinematic();}

public:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	physics::IRigidEntity* RigidEntity;
};