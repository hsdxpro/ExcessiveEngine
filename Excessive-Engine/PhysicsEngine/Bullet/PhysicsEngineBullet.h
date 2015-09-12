#pragma once


#include "../IPhysicsEngine.h"
#include "../IRigidBodyEntity.h"

#include "RigidBodyEntity.h"
#include "SoftBodyEntity.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"

#include "SupportLibrary/BasicTypes.h"
#include "mymath/mymath.h"
#include <vector>
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

	bool TraceClosestPoint(const mm::vec3& from, const mm::vec3& to, physics::rTraceResult& traceResult_out, const physics::rTraceParams& params = physics::rTraceParams()) override;

	// Create, Add DYNAMIC rigid body to physics world
	physics::IRigidBodyEntity* AddEntityRigidDynamic(mm::vec3* vertices, u32 nVertices, float mass = 1) override;

	// Create, Add STATIC rigid body to physics world
	physics::IRigidBodyEntity* AddEntityRigidStatic(mm::vec3* vertices, u32 nVertices, void* indices, u32 indexStride, u32 nIndices) override;

	// Create, Add capsule rigid body to physics world
	physics::IRigidBodyEntity* AddEntityRigidCapsule(float height, float radius, float mass) override;

	bool RemoveEntity(physics::IRigidBodyEntity* e) override;

	void SetLayerCollision(size_t ID0, size_t ID1, bool bEnableCollision) override;

	__inline bool IsLayersCanCollide(size_t ID0, size_t ID1) const override
	{
		assert(ID0 < sqrt(layerCollisionMatrix.size()));
		assert(ID1 < sqrt(layerCollisionMatrix.size()));

		return layerCollisionMatrix[ID0 + ID1 * nLayerCollisionMatrixRows] > 0;
	}

	__inline std::vector<physics::rCollision>& GetCollisionList() override { return contactList; }

	bool GetDebugData(mm::vec3*& linesFromNonUniqPoints_out, size_t& nLines_out) const override;

	//__inline bool IsOverlapCallbacksEnabled(){ return bOverlapCallbacksEnabled; }
private:
	btDiscreteDynamicsWorld* world;

	std::vector<physics::IRigidBodyEntity*> entities;

	std::vector<physics::rCollision> contactList;

	// byte bool array bitch pls
	std::vector<u8> layerCollisionMatrix;
	size_t nLayerCollisionMatrixRows;
};


class BulletCollisionDispatcher : public btCollisionDispatcher
{
public:
	BulletCollisionDispatcher(btCollisionConfiguration* c, PhysicsEngineBullet* p)
	:btCollisionDispatcher(c), physicsEngine(p)
	{

	}

	bool needsCollision(const btCollisionObject* bodyA, const btCollisionObject* bodyB) override
	{
		bool bCollide = btCollisionDispatcher::needsCollision(bodyA, bodyB);

		i64 colGroupA;
		i64 colGroupB;

		if (bCollide)
		{
			if (!bodyA->getCollisionShape()->isSoftBody())
				colGroupA = ((RigidBodyEntity*)bodyA->getUserPointer())->GetCollisionGroup();
			else 
				colGroupA = ((SoftBodyEntity*)bodyA->getUserPointer())->GetCollisionGroup();

			if (!bodyB->getCollisionShape()->isSoftBody())
				colGroupB = ((RigidBodyEntity*)bodyB->getUserPointer())->GetCollisionGroup();
			else
				colGroupB = ((SoftBodyEntity*)bodyB->getUserPointer())->GetCollisionGroup();
		}

		bCollide &= colGroupA == -1 || colGroupB == -1 || physicsEngine->IsLayersCanCollide(colGroupA, colGroupB);

		return bCollide;
	}

protected:
	PhysicsEngineBullet* physicsEngine;
};


struct ExcessiveClosestRayCallb : public btCollisionWorld::ClosestRayResultCallback
{
	ExcessiveClosestRayCallb(IPhysicsEngine* p, const btVector3& rayFromWorld, const btVector3& rayToWorld, const std::vector<size_t>& ignoredCollisionLayers)
	:btCollisionWorld::ClosestRayResultCallback(rayFromWorld, rayToWorld), ignoredCollisionLayers(ignoredCollisionLayers), physicsEngine(p)
	{
	}

	virtual bool needsCollision(btBroadphaseProxy* proxy0) const override
	{
		btCollisionObject* col = (btCollisionObject*)proxy0->m_clientObject;

		size_t colGroup;
		if (!col->getCollisionShape()->isSoftBody())
			colGroup = ((RigidBodyEntity*)col->getUserPointer())->GetCollisionGroup();
		else
			colGroup = ((SoftBodyEntity*)col->getUserPointer())->GetCollisionGroup();
		
		size_t i = 0;
		for (; i < ignoredCollisionLayers.size(); i++)
			if (ignoredCollisionLayers[i] == colGroup)
				break;

		return i == ignoredCollisionLayers.size();
	}

protected:
	IPhysicsEngine* physicsEngine;

	std::vector<size_t> ignoredCollisionLayers;
};