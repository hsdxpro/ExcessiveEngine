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
	void SetScale(const mm::vec3& v);

	const mm::vec3& GetPos();
	const mm::quat& GetRot();
	const mm::vec3& GetScale();

public:
	void _InnerUpdatePos() override;
	void _InnerUpdateRot() override;
	void _InnerUpdateScale() override;
	void _InnerUpdateSkew() override;

protected:
	physics::IEntityRigid* entityRigid;
};