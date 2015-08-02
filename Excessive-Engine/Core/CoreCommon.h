#pragma once
#include "PhysicsEngine\PhysicsCommon.h"

class Actor;
struct rCollision
{
	Actor*  self;
	Actor*  other;

	physics::IRigidBodyEntity* selfBody;
	physics::IRigidBodyEntity* otherBody;

	std::vector<rContactPoint> contacts;
};
