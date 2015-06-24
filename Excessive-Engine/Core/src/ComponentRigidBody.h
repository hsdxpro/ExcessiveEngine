#pragma once
#include "WorldComponent.h"

namespace physics { class IEntityRigid;  }

class ComponentRigidBody : public WorldComponent
{
public:
	ComponentRigidBody(physics::IEntityRigid* e) :entityRigid(e){}

	void UpdateAfterPhysicsSimulate(); // After physics simulation done, we transforms all WorldComponent child also

	void SetPos(const mm::vec3& v);
	void SetRot(const mm::quat& q);
	void SetScaleLocal(const mm::vec3& v);

	const mm::vec3& GetPos();
	const mm::quat& GetRot();
	const mm::vec3& GetScaleLocal();

public:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	physics::IEntityRigid* entityRigid;
};