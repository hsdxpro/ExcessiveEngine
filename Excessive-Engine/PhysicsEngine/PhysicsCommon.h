#pragma once
#include "..\..\Externals\include\mymath\mymath.h"
#include <vector>

namespace physics { class IRigidBodyEntity; }

struct rContactPoint
{
	mm::vec3 normalA;
	mm::vec3 normalB;
	mm::vec3 posA;
	mm::vec3 posB;
};

struct rPhysicsCollision
{
	physics::IRigidBodyEntity* entityA;
	physics::IRigidBodyEntity* entityB;

	std::vector<rContactPoint> contacts;
};

struct rPhysicsTraceInfo
{
	void* userPointer;

	mm::vec3 pos;
	mm::vec3 normal;
};