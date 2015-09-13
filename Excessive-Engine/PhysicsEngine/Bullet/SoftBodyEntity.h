#pragma once
#include "..\ISoftBodyEntity.h"
#include "BulletSoftBody\btSoftBody.h"

#include "SupportLibrary\BasicTypes.h"

class btSoftBody;

namespace physics { namespace bullet {

class SoftBodyEntity : public physics::ISoftBodyEntity
{
public:
	SoftBodyEntity(btSoftBody* body);

	__inline i64 GetCollisionGroup() const override  { return collisionGroupID; }

	__inline btSoftBody* GetBody() const { return body; }

protected:
	btSoftBody* body;

	i64 collisionGroupID;
	void* userPointer;
};

}} // Namespace end  physics::bullet