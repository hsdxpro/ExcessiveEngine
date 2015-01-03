#pragma once
#include "../Common/src/BasicTypes.h"
#include "mymath/mymath.h"

namespace physics {

class IEntity;

class IEngine
{
public:
	virtual void release() = 0;

	virtual void update(float deltaTime) = 0;

	// Create, add DYNAMIC rigid body to physics world
	virtual physics::IEntity* addEntityRigidDynamic(mm::vec3* vertices, u32 nVertices, float mass = 1) = 0;

	// Create, add STATIC rigid body to physics world
	virtual physics::IEntity* addEntityRigidStatic(mm::vec3* vertices, u32 nVertices, void* indices, u32 indexSize, u32 nIndices) = 0;
};

} 