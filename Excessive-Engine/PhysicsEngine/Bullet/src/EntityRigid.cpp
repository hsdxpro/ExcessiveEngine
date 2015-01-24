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

void EntityRigid::setRot(const mm::quat& r)
{
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	trans.setRotation(btQuaternion(r.scalar(), r.vector().x, r.vector().y, r.vector().z));

	body->setWorldTransform(trans);
	body->getMotionState()->setWorldTransform(trans);
}

mm::vec3 EntityRigid::getPos() {
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	
	auto& vec = trans.getOrigin();
	return mm::vec3(vec.x(), vec.y(), vec.z());
}

mm::quat EntityRigid::getRot() {
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);

	auto& btRot = trans.getRotation();
	mm::quat rot;
		rot.value.y = btRot.y();
		rot.value.x = btRot.x();
		rot.value.z = btRot.z();
		rot.value.w = btRot.w();
	return rot;
}