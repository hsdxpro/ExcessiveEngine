#pragma once
#include "PhysicsEngine\PhysicsCommon.h"

class Actor;
struct rCollision
{
	rCollision(): self(0), other(0), selfBody(0), otherBody(0){}

	Actor*  self;
	Actor*  other;

	physics::IRigidBodyEntity* selfBody;
	physics::IRigidBodyEntity* otherBody;

	std::vector<rContactPoint> contacts;
};
