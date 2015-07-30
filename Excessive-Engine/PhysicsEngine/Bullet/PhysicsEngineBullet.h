#pragma once
#include "../IPhysicsEngine.h"
#include "../IRigidBodyEntity.h"

#include "SupportLibrary/BasicTypes.h"
#include "mymath/mymath.h"
#include <vector>
#include "RigidBodyEntity.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"

using namespace physics::bullet;

class btSoftRigidDynamicsWorld;
class btDiscreteDynamicsWorld;

struct rPhysicsEngineBullet 
{
	mm::vec3 gravity;
};

class PhysicsEngineBullet : public IPhysicsEngine
{
public:
	PhysicsEngineBullet(const rPhysicsEngineBullet& d);
	~PhysicsEngineBullet();
	void Release() override;

	void Update(float deltaTime) override;

	// Create, Add DYNAMIC rigid body to physics world
	physics::IRigidBodyEntity* AddEntityRigidDynamic(mm::vec3* vertices, u32 nVertices, float mass = 1) override;

	// Create, Add STATIC rigid body to physics world
	physics::IRigidBodyEntity* AddEntityRigidStatic(mm::vec3* vertices, u32 nVertices, void* indices, u32 indexStride, u32 nIndices) override;

	// Create, Add capsule rigid body to physics world
	physics::IRigidBodyEntity* AddEntityRigidCapsule(float height, float radius, float mass) override;

	bool RemoveEntity(physics::IRigidBodyEntity* e) override;

	void SetLayerCollision(size_t ID0, size_t ID1, bool bEnableCollision) override;

	__inline bool CheckLayerCollision(size_t ID0, size_t ID1) const override
	{
		assert(ID0 < sqrt(layerCollisionMatrix.size()));
		assert(ID1 < sqrt(layerCollisionMatrix.size()));

		return layerCollisionMatrix[ID0 + ID1 * nLayerCollisionMatrixRows] > 0;
	}

	__inline std::vector<rPhysicsCollision>& GetCollisionList() override { return contactList; }

	bool GetDebugData(mm::vec3*& linesFromNonUniqPoints_out, size_t& nLines_out) const override;

private:
	btDiscreteDynamicsWorld* world;

	std::vector<physics::IRigidBodyEntity*> entities;

	std::vector<rPhysicsCollision> contactList;

	// byte bool array bitch pls
	std::vector<u8> layerCollisionMatrix;
	size_t nLayerCollisionMatrixRows;
};

class BulletBroadPhaseCallback : public btOverlapFilterCallback
{
public:
	BulletBroadPhaseCallback(PhysicsEngineBullet* e) :physicsEngine(e){}

	// return true when pairs need collision
	virtual bool	needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
	{
		
		RigidBodyEntity* entityA = (RigidBodyEntity*)((btCollisionObject*)proxy0->m_clientObject)->getUserPointer();
		RigidBodyEntity* entityB = (RigidBodyEntity*)((btCollisionObject*)proxy1->m_clientObject)->getUserPointer();

		if (!entityA || !entityB)
			return true;

		return physicsEngine->CheckLayerCollision(entityA->GetCollisionGroup(), entityB->GetCollisionGroup());
	}
protected:
	PhysicsEngineBullet* physicsEngine;
};