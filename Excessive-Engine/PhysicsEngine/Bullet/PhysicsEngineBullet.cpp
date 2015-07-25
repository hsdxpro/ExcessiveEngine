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
	world = new btDiscreteDynamicsWorld(new	btCollisionDispatcher(new btDefaultCollisionConfiguration),
										//new btDbvtBroadphase,
										new btAxisSweep3({ -1000, -1000, -1000 }, { 1000, 1000, 1000 }),
										new btSequentialImpulseConstraintSolver,
										new btDefaultCollisionConfiguration);

	//world->setGravity(btVector3(0, 0, -9.81));
	world->setGravity(btVector3(0, 0, -9.81));
}

PhysicsEngineBullet::~PhysicsEngineBullet()
{
}

void PhysicsEngineBullet::Release()
{
	delete this;
}

void PhysicsEngineBullet::Update(float deltaTime)
{
	world->stepSimulation(1.f / 30, 5);

	//contactList.clear();

	int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* colA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject* colB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance() < 0.f)
			{
				// Okay bit brute force (tmp)
				//i64* collisionGroupA = dynamic_cast<RigidBodyEntity*>(colA->getUserPointer());
				//i64* collisionGroupB = (i64*)colB->getUserPointer();

				//rContactInfo info;
					
				//contactList.push_back(info);

				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
			}
		}
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

void PhysicsEngineBullet::GetDebugData(mm::vec3* nonIndexedVertices, uint32_t vertsByteSize, uint32_t& nVertices)
{
	nVertices = 0;
	const btCollisionObjectArray& colObjArray = world->getCollisionWorld()->getCollisionObjectArray();
	u32 nObjs = world->getNumCollisionObjects();

	for (size_t i = 0; i < nObjs; i++)
	{
		btCollisionShape* colShape = colObjArray[i]->getCollisionShape();
		btTransform worldTrans = colObjArray[i]->getWorldTransform();

		if (colShape->isPolyhedral())
		{
			int asd = 5;
			asd++;
		}

		if (colShape->isCompound())
		{
			int asd = 5;
			asd++;
		}

		if (colShape->isConcave())
		{
			int asd = 5;
			asd++;
		}

		if (colShape->isInfinite())
		{
			int asd = 5;
			asd++;
		}

		// Add each edge from convex Shape to the list
		if (colShape->isConvex())
		{
			btConvexHullShape* col = (btConvexHullShape*)colShape;

			u32 nVerts = col->getNumVertices();

			for (u32 j = 0; j < nVerts; j++)
			{
				btVector3 vtx;
				col->getVertex(j, vtx);
				vtx = worldTrans * vtx;
				nonIndexedVertices[nVertices + j] = { vtx.x(), vtx.y(), vtx.z() };
			}
			nVertices += nVerts;
		}
	}
}