#include "PhysicsEngineBullet.h"

// Common bullet include
#include "Bullet3/btBulletDynamicsCommon.h"

// Soft body include
#include "Bullet3/BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "Bullet3/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "Bullet3/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "Bullet3/BulletSoftBody/btSoftBodyHelpers.h"
#include "Bullet3/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

#include "EntityRigid.h"

////////////////////////////////////////////////////////////////////////////////
// Export Create function
//
////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C"
EXPORT physics::IEngine* CreatePhysicsEngineBullet(const rPhysicsEngineBullet& d) 
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

	// Contact mainfolds for debugging
	//auto overlappingPairCache = world->GetBroadphase()->GetOverlappingPairCache();
	//auto nPairs = overlappingPairCache->GetNumOverlappingPairs();
	//for (u32 i = 0; i < nPairs; i++)
	//{
	//	btManifoldArray array;
	//	overlappingPairCache->GetOverlappingPairArray()[i].m_algorithm->GetAllContactManifolds(array);
	//	for (u32 j = 0; j < array.size(); j++)
	//	{
	//		auto& maniFold = array.at(j);
	//	}
	//}
}

physics::IEntityRigid* PhysicsEngineBullet::AddEntityRigidDynamic(mm::vec3* vertices, u32 nVertices, float mass /*= 1*/) 
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
	world->addRigidBody(body);
		body->setFriction(1);

	EntityRigid* e = new EntityRigid(body);
		entities.push_back(e);
	return e;
}

physics::IEntityRigid* PhysicsEngineBullet::AddEntityRigidStatic(mm::vec3* vertices, u32 nVertices, void* indices, u32 indexSize, u32 nIndices) 
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

	EntityRigid* e = new EntityRigid(body);
		entities.push_back(e);
	return e;
}

physics::IEntityRigid* PhysicsEngineBullet::AddEntityRigidCapsule(float height, float radius, float mass)
{
	btCapsuleShapeZ* capsuleShape = new btCapsuleShapeZ(radius, height);
	capsuleShape->setSafeMargin(0, 0); // Thanks convex hull for your imprecision...

	btVector3 localInertia(0, 0, 0);
	if (mass != 0)
		capsuleShape->calculateLocalInertia(mass, localInertia);

	// Create rigid body
	btRigidBody* body = new btRigidBody(mass, new btDefaultMotionState(), capsuleShape, localInertia);
	world->addRigidBody(body);

	EntityRigid* e = new EntityRigid(body);
		entities.push_back(e);
	return e;
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