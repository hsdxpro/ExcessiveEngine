#pragma once
#include "../Common/src/BasicTypes.h"
#include "mymath/mymath.h"

namespace physics {

class IEntityRigid;

class IEngine
{
public:
	virtual void release() = 0;

	virtual void update(float deltaTime) = 0;

	// Create, add DYNAMIC rigid body to physics world
	virtual physics::IEntityRigid* addEntityRigidDynamic(mm::vec3* vertices, u32 nVertices, float mass = 1) = 0;

	// Create, add STATIC rigid body to physics world
	virtual physics::IEntityRigid* addEntityRigidStatic(mm::vec3* vertices, u32 nVertices, void* indices, u32 indexSize, u32 nIndices) = 0;

	// Create, add capsule rigid body to physics world
	virtual physics::IEntityRigid* addEntityRigidCapsule(float height, float radius, float mass) = 0;

	virtual void GetDebugData(mm::vec3* nonIndexedVertices, uint32_t vertsByteSize, uint32_t& nVertices) = 0;
};

} 