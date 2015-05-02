#include "EntityRigid.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"

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

void EntityRigid::setScale(const mm::vec3& s)
{
	btCollisionShape* shape = body->getCollisionShape();

	if (shape)
		shape->setLocalScaling(btVector3(s.x, s.y, s.z));
}

mm::vec3 EntityRigid::getPos() 
{
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	
	auto& vec = trans.getOrigin();
	return mm::vec3(vec.x(), vec.y(), vec.z());
}

mm::quat EntityRigid::getRot()
{
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);

	auto& btRot = trans.getRotation();
	mm::quat rot;
		rot.value.x = btRot.x();
		rot.value.y = btRot.y();
		rot.value.z = btRot.z();
		rot.value.w = btRot.w();
	return rot;
}

mm::vec3 EntityRigid::getScale()
{
	btCollisionShape* shape = body->getCollisionShape();

	if (shape)
	{
		const btVector3& scale = shape->getLocalScaling();
		return mm::vec3(scale.x(), scale.y(), scale.z());
	}
	return mm::vec3(0, 0, 0);
}