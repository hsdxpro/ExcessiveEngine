#pragma once
#include "WorldComponent.h"

namespace physics { class IEntityRigid;  }

class ComponentRigidBody : public WorldComponent
{
public:
	ComponentRigidBody(physics::IEntityRigid* e) :entityRigid(e){}

	void updateAfterPhysicsSimulate(); // After physics simulation done, we transforms all WorldComponent child also

	void setPos(const mm::vec3& v);
	void setRot(const mm::quat& q);
	void setScale(const mm::vec3& v);

	const mm::vec3& getPos();
	const mm::quat& getRot();
	const mm::vec3& getScale();

public:
	void _innerUpdatePos() override;
	void _innerUpdateRot() override;
	void _innerUpdateScale() override;

protected:
	physics::IEntityRigid* entityRigid;
};