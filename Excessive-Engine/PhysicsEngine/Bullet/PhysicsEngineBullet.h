#pragma once
#include "../IEngine.h"
#include "../IEntityRigid.h"

#include "SupportLibrary/BasicTypes.h"
#include "mymath/mymath.h"
#include <vector>


class btSoftRigidDynamicsWorld;
class btDiscreteDynamicsWorld;

struct rPhysicsEngineBullet 
{

};

class PhysicsEngineBullet : public physics::IEngine
{
public:
	PhysicsEngineBullet(const rPhysicsEngineBullet& d);
	~PhysicsEngineBullet();
	void Release() override;

	void Update(float deltaTime) override;

	// Create, Add DYNAMIC rigid body to physics world
	physics::IEntityRigid* AddEntityRigidDynamic(mm::vec3* vertices, u32 nVertices, float mass = 1) override;

	// Create, Add STATIC rigid body to physics world
	physics::IEntityRigid* AddEntityRigidStatic(mm::vec3* vertices, u32 nVertices, void* indices, u32 indexStride, u32 nIndices) override;

	// Create, Add capsule rigid body to physics world
	physics::IEntityRigid* AddEntityRigidCapsule(float height, float radius, float mass);

	void GetDebugData(mm::vec3* nonIndexedVertices, uint32_t vertsByteSize, uint32_t& nVertices) override;

private:
	btDiscreteDynamicsWorld* world;

	std::vector<physics::IEntityRigid*> entities;
};

