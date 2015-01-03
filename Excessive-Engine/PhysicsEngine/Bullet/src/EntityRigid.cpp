#include "EntityRigid.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"

EntityRigid::EntityRigid(btRigidBody* body) 
:body(body) {
}

void EntityRigid::setPos(const mm::vec3& p) {
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	trans.setOrigin(btVector3(p.x, p.y, p.z));

	body->setWorldTransform(trans);
	body->getMotionState()->setWorldTransform(trans);
}

mm::vec3 EntityRigid::getPos() {
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	
	auto& vec = trans.getOrigin();
	return mm::vec3(vec.x(), vec.y(), vec.z());
}