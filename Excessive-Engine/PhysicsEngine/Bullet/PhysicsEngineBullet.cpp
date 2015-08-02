#include "PhysicsEngineBullet.h"

// Common bullet include
#include "Bullet3/btBulletDynamicsCommon.h"

// Soft body include
#include "Bullet3/BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "Bullet3/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "Bullet3/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "Bullet3/BulletSoftBody/btSoftBodyHelpers.h"
#include "Bullet3/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

#include "RigidBodyEntity.h"
#include "PhysicsEngineBulletDebugGatherer.h"
#include "SupportLibrary/VisualCpuProfiler.h"

////////////////////////////////////////////////////////////////////////////////
// Export Create function
//
////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

using namespace physics::bullet;

extern "C"
EXPORT IPhysicsEngine* CreatePhysicsEngineBullet(const rPhysicsEngineBullet& d) 
{
	return new PhysicsEngineBullet(d);
}

PhysicsEngineBullet::PhysicsEngineBullet(const rPhysicsEngineBullet& d) 
{
	world = new btDiscreteDynamicsWorld(new	BulletCollisionDispatcher(new btDefaultCollisionConfiguration, this),
										new btDbvtBroadphase,
										new btSequentialImpulseConstraintSolver,
										new btDefaultCollisionConfiguration);

	world->setGravity(btVector3(d.gravity.x, d.gravity.y, d.gravity.z));

	btIDebugDraw* debugDrawer = (btIDebugDraw*)new PhysicsEngineBulletDebugGatherer();
	debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawAabb);

	world->setDebugDrawer(debugDrawer);

	// Populate collisionMatrix with true values, everything can collide with everything by default
	nLayerCollisionMatrixRows = 4;
	layerCollisionMatrix.resize(nLayerCollisionMatrixRows * nLayerCollisionMatrixRows);
	memset(layerCollisionMatrix.data(), 1, nLayerCollisionMatrixRows * nLayerCollisionMatrixRows);
}

PhysicsEngineBullet::~PhysicsEngineBullet()
{
	/* Clean up	*/
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject*	obj = world->getCollisionObjectArray()[i];
		btRigidBody*		body = btRigidBody::upcast(obj);

		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		while (world->getNumConstraints())
		{
			btTypedConstraint*	pc = world->getConstraint(0);
			world->removeConstraint(pc);
			delete pc;
		}
		btSoftBody* softBody = btSoftBody::upcast(obj);
		if (softBody)
		{
			//world->removeSoftBody(softBody);
		}
		else
		{
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body)
				world->removeRigidBody(body);
			else
				world->removeCollisionObject(obj);
		}
		world->removeCollisionObject(obj);
		delete obj;
	}

	delete world;
}

void PhysicsEngineBullet::Release()
{
	delete this;
}

void PhysicsEngineBullet::Update(float deltaTime)
{
	{
		PROFILE_SCOPE("Simulate");
		world->stepSimulation(deltaTime, 0);
	}
	
	{
		PROFILE_SCOPE("Contact List Query");
		contactList.clear();

		int numManifolds = world->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);

			int numContacts = contactManifold->getNumContacts();
			if (numContacts != 0)
			{
				const btCollisionObject* colA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
				const btCollisionObject* colB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

				// Fill up our structure with contact informations
				rPhysicsCollision colInfo;
				colInfo.entityA = (RigidBodyEntity*)colA->getUserPointer();
				colInfo.entityB = (RigidBodyEntity*)colB->getUserPointer();

				for (int j = 0; j < numContacts; j++)
				{
					btManifoldPoint& pt = contactManifold->getContactPoint(j);
					if (pt.getDistance() <= 0.f)
					{
						//Fill contact data
						rContactPoint c;
						c.normalA = -mm::vec3(pt.m_normalWorldOnB.x(), pt.m_normalWorldOnB.y(), pt.m_normalWorldOnB.z());
						c.normalB = mm::vec3(pt.m_normalWorldOnB.x(), pt.m_normalWorldOnB.y(), pt.m_normalWorldOnB.z());
						c.posA = mm::vec3(pt.m_positionWorldOnA.x(), pt.m_positionWorldOnA.y(), pt.m_positionWorldOnA.z());
						c.posB = mm::vec3(pt.m_positionWorldOnB.x(), pt.m_positionWorldOnB.y(), pt.m_positionWorldOnB.z());
						colInfo.contacts.push_back(c);

						const btVector3& ptA = pt.getPositionWorldOnA();
						const btVector3& ptB = pt.getPositionWorldOnB();
						const btVector3& normalOnB = pt.m_normalWorldOnB;
					}
				}

				if (colInfo.contacts.size() != 0)
					contactList.push_back(colInfo);
			}
		}
	}

	if (world->getDebugDrawer())
	{
		((PhysicsEngineBulletDebugGatherer*)world->getDebugDrawer())->ClearFrameData();
		world->debugDrawWorld();
	}
}

physics::IRigidBodyEntity* PhysicsEngineBullet::AddEntityRigidDynamic(mm::vec3* vertices, u32 nVertices, float mass /*= 1*/) 
{
	// You should call PhysicsEngineBullet::CreateEntityRigidStatic
	assert(mass != 0);

	// Create collision shape for rigid body, based on it's vertices and mass
	btConvexHullShape* colShape = new btConvexHullShape((btScalar*)vertices, nVertices, sizeof(mm::vec3));
	colShape->setSafeMargin(0, 0); // Thanks convex hull for your imprecision...

	btVector3 localInertia(0, 0, 0);
	if (mass != 0)
		colShape->calculateLocalInertia(mass, localInertia);

	// Create rigid body
	btRigidBody* body = new btRigidBody(mass, new btDefaultMotionState(), colShape, localInertia);
		body->setFriction(1);
	world->addRigidBody(body);

	RigidBodyEntity* e = new RigidBodyEntity(body);
		entities.push_back(e);
	return e;
}

physics::IRigidBodyEntity* PhysicsEngineBullet::AddEntityRigidStatic(mm::vec3* vertices, u32 nVertices, void* indices, u32 indexSize, u32 nIndices) 
{
	btTriangleIndexVertexArray* VBIB;

	// Indices need copy, signed unsigne differences -.-
	// Okay bullet vector container equal size, let bullet read out pointer
	if (sizeof(mm::vec3) == sizeof(btVector3)) 
	{
		VBIB = new btTriangleIndexVertexArray(nIndices / 3, (int*)indices, 3 * indexSize, nVertices, (btScalar*)vertices, sizeof(btVector3));
	} 
	else // Bullshit if mm::vec3 and btVector3 size not equal, we need into new array, mm::vec3.xyz to btVector3
	{

		int* myIndices = new int[nIndices];
		for (u32 i = 0; i < nIndices; i++)
			myIndices[i] = (int)*((u32*)(indices) + i);

		btVector3* vertices_memCorrected = new btVector3[nVertices];
		for (u32 i = 0; i < nVertices; i++)
			memcpy(vertices_memCorrected[i], (unsigned char*)vertices + i * sizeof(mm::vec3), sizeof(mm::vec3));

		VBIB = new btTriangleIndexVertexArray(nIndices / 3, (int*)myIndices, 3 * indexSize, nVertices, (btScalar*)vertices_memCorrected, sizeof(btVector3));
	}
	
	// Create rigid body
	btRigidBody* body = new btRigidBody(0, new btDefaultMotionState(), new btBvhTriangleMeshShape(VBIB, true), btVector3(0, 0, 0));
	world->addRigidBody(body);

	RigidBodyEntity* e = new RigidBodyEntity(body);
		entities.push_back(e);
	return e;
}

physics::IRigidBodyEntity* PhysicsEngineBullet::AddEntityRigidCapsule(float height, float radius, float mass)
{
	btCapsuleShapeZ* capsuleShape = new btCapsuleShapeZ(radius, height);
	capsuleShape->setSafeMargin(0, 0); // Thanks convex hull for your imprecision...

	btVector3 localInertia(0, 0, 0);
	if (mass != 0)
		capsuleShape->calculateLocalInertia(mass, localInertia);

	// Create rigid body
	btRigidBody* body = new btRigidBody(mass, new btDefaultMotionState(), capsuleShape, localInertia);
	world->addRigidBody(body);

	RigidBodyEntity* e = new RigidBodyEntity(body);
		entities.push_back(e);
	return e;
}

bool PhysicsEngineBullet::RemoveEntity(physics::IRigidBodyEntity* e)
{
	auto it = std::find(entities.begin(), entities.end(), e);
	if (it != entities.end())
	{
		auto entity = (RigidBodyEntity*)*it;
		auto rigidBody = entity->GetBody();

		entities.erase(it);
		world->removeRigidBody(rigidBody);

		delete entity->GetBody();
		delete entity;
	}
	return false;
}

void PhysicsEngineBullet::SetLayerCollision(size_t ID0, size_t ID1, bool bEnableCollision)
{
	if (ID0 > nLayerCollisionMatrixRows - 1 || ID1 > nLayerCollisionMatrixRows - 1)
	{
		// Reallocate larger matrix
		size_t nRows = std::max(ID0, ID1) + 1;
		layerCollisionMatrix.resize(nRows * nRows);

		// Move old datas to correct places
		// i = 0 will not run, cuz 0th will remain good in memory
		for (size_t i = nLayerCollisionMatrixRows - 1; i > 0; i--)
		{
			u8* src = i * nLayerCollisionMatrixRows + (u8*)layerCollisionMatrix.data();
			u8* dst = src + i * (nRows - nLayerCollisionMatrixRows);
			memmove(dst, src, nLayerCollisionMatrixRows);

			// Set newly allocated bytes to 1 (part0)
			// [src, dst[ set 1 these byte are the newly allocated ones, ID0 can collide with everything, and ID1 also
			memset(src, 1, dst - src);
		}

		// Set newly allocated bytes to 1 (part1)
		size_t asd = nRows + (nRows - nLayerCollisionMatrixRows);
		memset((u8*)layerCollisionMatrix.data() + (nRows *  nRows) - asd, 1, asd);

		nLayerCollisionMatrixRows = nRows;
	}

	layerCollisionMatrix[ID0 + nLayerCollisionMatrixRows * ID1] = bEnableCollision;
	layerCollisionMatrix[ID1 + nLayerCollisionMatrixRows * ID0] = bEnableCollision;
}

bool PhysicsEngineBullet::GetDebugData(mm::vec3*& linesFromNonUniqPoints_out, size_t& nLines_out) const
{
	PhysicsEngineBulletDebugGatherer* debugInfoGatherer = (PhysicsEngineBulletDebugGatherer*)world->getDebugDrawer();
	if (debugInfoGatherer)
	{
		debugInfoGatherer->GetDebugData(linesFromNonUniqPoints_out, nLines_out);
		return true;
	}
	
	return false;
}