#include "PhysicsEngineBullet.h"


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
}

PhysicsEngineBullet::~PhysicsEngineBullet() {
}

void PhysicsEngineBullet::release() {
	delete this;
}

void PhysicsEngineBullet::update(float deltaTime) {
	
}