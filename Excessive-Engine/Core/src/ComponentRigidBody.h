#pragma once
#include "WorldComponent.h"

namespace physics { class IEntityRigid;  }

class ComponentRigidBody : public WorldComponent
{
public:
	ComponentRigidBody(physics::IEntityRigid* a);

	void UpdateAfterPhysicsSimulate(); // After physics simulation done, we transforms all WorldComponent child also

	void SetAngularFactor(float factor);
	void SetKinematic();

public:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	physics::IEntityRigid* entityRigid;
};