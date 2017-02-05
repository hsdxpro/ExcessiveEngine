#pragma once
#include "PhysicsEngine/ISoftBodyEntity.h"
#include "BulletSoftBody\btSoftBody.h"

#include "BaseLibrary\Types.h"

class btSoftBody;

namespace physics { namespace bullet {

class SoftBodyEntity : public physics::ISoftBodyEntity
{
public:
	SoftBodyEntity(btSoftBody* body);

	inline u64 GetCollisionGroup() const override  { return collisionGroupID; }

	inline btSoftBody* GetBody() const { return body; }

protected:
	btSoftBody* body;

	u64 collisionGroupID;
	void* userPointer;
};

}} // Namespace end  physics::bullet