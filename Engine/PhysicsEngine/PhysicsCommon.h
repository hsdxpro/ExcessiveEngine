#pragma once
//#include "IRigidBodyEntity.h"
//#include "ISoftBodyEntity.h"

#include "..\..\Externals\include\mymath\mymath.h"

#include <vector>

namespace physics
{

class IRigidBodyEntity;
class ISoftBodyEntity;

struct ContactPoint
{
	mm::vec3 normalA;
	mm::vec3 normalB;
	mm::vec3 posA;
	mm::vec3 posB;
};

struct Collision
{
	IRigidBodyEntity* rigidBodyA;
	IRigidBodyEntity* rigidBodyB;

	ISoftBodyEntity* softBodyA;
	ISoftBodyEntity* softBodyB;

	std::vector<ContactPoint> contacts;
};

struct TraceResult
{
	void* userPointer;

	mm::vec3 pos;
	mm::vec3 normal;
};

struct TraceParams
{
	inline void AddIgnoreCollisionLayer(size_t collisionLayerID) 
	{ 
		ignoredCollisionLayers.push_back(collisionLayerID);
	}
	
	std::vector<size_t> ignoredCollisionLayers;
};

} // namespace physics