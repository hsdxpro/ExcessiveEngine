#pragma once
#include "../IPhysicsEngine.h"
#include "../IRigidBodyEntity.h"

#include "SupportLibrary/BasicTypes.h"
#include "mymath/mymath.h"
#include <vector>
#include "RigidBodyEntity.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"

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

	bool TraceClosestPoint(const mm::vec3& from, const mm::vec3& to, rPhysicsTraceInfo& traceInfo_out) override;

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

	//__inline bool IsOverlapCallbacksEnabled(){ return bOverlapCallbacksEnabled; }
private:
	btDiscreteDynamicsWorld* world;

	std::vector<physics::IRigidBodyEntity*> entities;

	std::vector<rPhysicsCollision> contactList;

	// byte bool array bitch pls
	std::vector<u8> layerCollisionMatrix;
	size_t nLayerCollisionMatrixRows;
};


class BulletCollisionDispatcher : public btCollisionDispatcher
{
public:
	BulletCollisionDispatcher(btCollisionConfiguration* collisionConfiguration, PhysicsEngineBullet* physicsEngine)
	:btCollisionDispatcher(collisionConfiguration), physicsEngine(physicsEngine)
	{

	}

	bool needsCollision(const btCollisionObject* body0, const btCollisionObject* body1) override
	{
		bool bCollide = btCollisionDispatcher::needsCollision(body0, body1);

		if (bCollide)
		{
			RigidBodyEntity* entityA = (RigidBodyEntity*)body0->getUserPointer();
			RigidBodyEntity* entityB = (RigidBodyEntity*)body1->getUserPointer();
			
			bCollide &= entityB->GetCollisionGroup() == -1 || entityA->GetCollisionGroup() == -1 || physicsEngine->CheckLayerCollision(entityA->GetCollisionGroup(), entityB->GetCollisionGroup());
		}

		return bCollide;
	}

protected:
	PhysicsEngineBullet* physicsEngine;
};