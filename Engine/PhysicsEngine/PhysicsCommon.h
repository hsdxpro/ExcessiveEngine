#pragma once
#include "IRigidBodyEntity.h"
#include "ISoftBodyEntity.h"

#include "..\..\Externals\include\mymath\mymath.h"
#include <vector>

namespace physics
{

struct rContactPoint
{
	mm::vec3 normalA;
	mm::vec3 normalB;
	mm::vec3 posA;
	mm::vec3 posB;
};

struct rCollision
{
	physics::IRigidBodyEntity* rigidBodyA;
	physics::IRigidBodyEntity* rigidBodyB;

	physics::ISoftBodyEntity* softBodyA;
	physics::ISoftBodyEntity* softBodyB;

	std::vector<rContactPoint> contacts;
};

struct rTraceResult
{
	void* userPointer;

	mm::vec3 pos;
	mm::vec3 normal;
};

struct rTraceParams
{
	void AddIgnoreCollisionLayer(size_t collisionLayerID) { ignoredCollisionLayers.push_back(collisionLayerID); }
	
	std::vector<size_t> ignoredCollisionLayers;
};

} // namespace physics