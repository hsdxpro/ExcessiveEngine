#pragma once
#include "WorldComponent.h"
#include "..\PhysicsEngine\Interfaces\IEntityRigid.h"


class ComponentRigidBody : public WorldComponent
{
public:
	ComponentRigidBody(physics::IEntityRigid* a);

	void UpdateAfterPhysicsSimulate(); // After physics simulation done, we transforms all WorldComponent child also

	__inline void AddForce(const mm::vec3& force, const mm::vec3& relPos = {0,0,0}) {entityRigid->AddForce(force, relPos);}

	__inline void SetAngularFactor(float factor)	{entityRigid->SetAngularFactor(factor);}
	__inline void SetKinematic()					{entityRigid->SetKinematic();}

public:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	physics::IEntityRigid* entityRigid;
};