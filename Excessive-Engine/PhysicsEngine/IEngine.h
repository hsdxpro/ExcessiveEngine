#pragma once
#include "../Common/src/BasicTypes.h"
#include "mymath/mymath.h"

namespace physics {

class IEntityRigid;

class IEngine
{
public:
	virtual void Release() = 0;

	virtual void Update(float deltaTime) = 0;

	// Create, Add DYNAMIC rigid body to physics world
	virtual physics::IEntityRigid* AddEntityRigidDynamic(mm::vec3* vertices, u32 nVertices, float mass = 1) = 0;

	// Create, Add STATIC rigid body to physics world
	virtual physics::IEntityRigid* AddEntityRigidStatic(mm::vec3* vertices, u32 nVertices, void* indices, u32 indexSize, u32 nIndices) = 0;

	// Create, Add capsule rigid body to physics world
	virtual physics::IEntityRigid* AddEntityRigidCapsule(float height, float radius, float mass) = 0;

	virtual void GetDebugData(mm::vec3* nonIndexedVertices, uint32_t vertsByteSize, uint32_t& nVertices) = 0;
};

} 