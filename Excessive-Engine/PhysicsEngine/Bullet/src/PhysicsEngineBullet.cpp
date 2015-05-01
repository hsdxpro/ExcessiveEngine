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
EXPORT physics::IEngine* CreatePhysicsEngineBullet(const rPhysicsEngineBullet& d) {
	return new PhysicsEngineBullet(d);
}

PhysicsEngineBullet::PhysicsEngineBullet(const rPhysicsEngineBullet& d) {
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btSoftBodyRigidBodyCollisionConfiguration* collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	btVector3 worldMin(-1000, -1000, -1000);
	btVector3 worldMax(1000, 1000, 1000);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btAxisSweep3(worldMin, worldMax);

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	world = new btSoftRigidDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	// GImpact dispatching registering, need for some special collision variances
	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);

	world->setGravity(btVector3(0, 0, -9.81));

	world->getDispatchInfo().m_useContinuous = true;
	world->getDispatchInfo().m_enableSPU = false;

	//btSoftBodyWorldInfo	softBodyWorldInfo;
	//	softBodyWorldInfo.air_density = (btScalar)1.2;
	//	softBodyWorldInfo.water_density = 0;
	//	softBodyWorldInfo.water_offset = 0;
	//	softBodyWorldInfo.water_normal = btVector3(0, 0, 1);
	//	softBodyWorldInfo.m_gravity = world->getGravity();
	//	softBodyWorldInfo.m_dispatcher = world->getDispatcher();
	//	softBodyWorldInfo.m_broadphase = world->getBroadphase();
	//	softBodyWorldInfo.m_sparsesdf.Initialize();
}

PhysicsEngineBullet::~PhysicsEngineBullet() {
}

void PhysicsEngineBullet::release() {
	delete this;
}

void PhysicsEngineBullet::update(float deltaTime) {
	//world->stepSimulation(deltaTime, 2, 1.f / 60);
	world->stepSimulation(deltaTime);
}

physics::IEntity* PhysicsEngineBullet::addEntityRigidDynamic(mm::vec3* vertices, u32 nVertices, float mass /*= 1*/) {

	// You should call PhysicsEngineBullet::createEntityRigidStatic
	assert(mass != 0);

	// Create collision shape for rigid body, based on it's vertices and mass
	btCollisionShape* colShape = new btConvexHullShape((btScalar*)vertices, nVertices, sizeof(mm::vec3));;

	btVector3 localInertia(0, 0, 0);
	colShape->calculateLocalInertia(mass, localInertia);

	// Create rigid body
	btRigidBody* body = new btRigidBody(mass, new btDefaultMotionState(), colShape, localInertia);
	world->addRigidBody(body);
	body->setLinearFactor(btVector3(1, 1, 1));

	EntityRigid* e = new EntityRigid(body);
	entities.push_back(e);
	return e;
}

physics::IEntity* PhysicsEngineBullet::addEntityRigidStatic(mm::vec3* vertices, u32 nVertices, void* indices, u32 indexSize, u32 nIndices) {

	btTriangleIndexVertexArray* VBIB;

	// Indices need copy, signed unsigne differences -.-
	// Okay bullet vector container equal size, let bullet read out pointer
	if (sizeof(mm::vec3) == sizeof(btVector3)) {
		VBIB = new btTriangleIndexVertexArray(nIndices / 3, (int*)indices, 3 * indexSize, nVertices, (btScalar*)vertices, sizeof(btVector3));
	} else { // Bullshit if mm::vec3 and btVector3 size not equal, we need into new array, mm::vec3.xyz to btVector3

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