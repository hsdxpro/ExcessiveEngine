#pragma once
#include "WorldComponent.h"

namespace physics { class IEntityRigid;  }

class ComponentRigidBody : public WorldComponent
{
public:
	static u8 componentTypeID;
	ComponentRigidBody(physics::IEntityRigid* e) :entityRigid(e){}

	void updateAfterPhysicsSimulate(); // After physics simulation done, we transforms all WorldComponent child also

	void setPos(const mm::vec3& v);
	void setRot(const mm::quat& q);
	void setScale(const mm::vec3& v);

	const mm::vec3& getPos();
	const mm::quat& getRot();
	const mm::vec3& getScale();

protected:
	physics::IEntityRigid* entityRigid;
};