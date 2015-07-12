#pragma once
#include "mymath/mymath.h"
#include "SupportLibrary/BasicTypes.h"

namespace physics {

class IRigidEntity;

class IEngine
{
public:
	virtual void Release() = 0;

	virtual void Update(float deltaTime) = 0;

	// Create, Add DYNAMIC rigid body to physics world
	virtual physics::IRigidEntity* AddEntityRigidDynamic(mm::vec3* vertices, u32 nVertices, float mass = 1) = 0;

	// Create, Add STATIC rigid body to physics world
	virtual physics::IRigidEntity* AddEntityRigidStatic(mm::vec3* vertices, u32 nVertices, void* indices, u32 indexSize, u32 nIndices) = 0;

	// Create, Add capsule rigid body to physics world
	virtual physics::IRigidEntity* AddEntityRigidCapsule(float height, float radius, float mass) = 0;

	virtual void GetDebugData(mm::vec3* nonIndexedVertices, uint32_t vertsByteSize, uint32_t& nVertices) = 0;
};

} 