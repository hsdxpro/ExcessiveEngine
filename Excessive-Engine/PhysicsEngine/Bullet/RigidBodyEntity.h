#pragma once
#include "..\IRigidBodyEntity.h"

#include "SupportLibrary\BasicTypes.h"
#include "BulletDynamics\Dynamics\btRigidBody.h"


namespace physics { namespace bullet {

class RigidBodyEntity : public physics::IRigidBodyEntity
{
public:
	RigidBodyEntity(btRigidBody* body);

	void AddForce(const mm::vec3& force, const mm::vec3& relPos = { 0, 0, 0 }) override;

	__inline void SetUserPointer(void* p) { userPointer = p; }

	void SetGravityScale(float s) override;
	void SetTrigger(bool bTrigger) override;
	void SetCollisionGroup(i64 ID) override;

	void SetAngularFactor(float f) override;
	void SetKinematic(bool b) override;
	void SetVelocity(const mm::vec3& v) override;

	void SetPos(const mm::vec3& v) override;
	void SetRot(const mm::quat& q) override;
	void SetScaleLocal(const mm::vec3& v) override;

	const mm::vec3 GetPos() const override;
	const mm::quat GetRot() const override;
	const mm::vec3 GetScaleLocal() const override;

	__inline i64 GetCollisionGroup() const override { return collisionGroupID; }
	__inline mm::vec3 GetVelocity() const { return mm::vec3(body->getLinearVelocity().x(), body->getLinearVelocity().y(), body->getLinearVelocity().z()); }
	__inline void* GetUserPointer() { return userPointer; }

	__inline bool IsTrigger() const override { return (body->getCollisionFlags() & btRigidBody::CF_NO_CONTACT_RESPONSE) != 0; }
	__inline bool IsStatic() const { return body->isStaticObject(); }
	__inline bool IsDynamic() const { return !body->isStaticObject(); }

	btRigidBody* GetBody();

protected:
	btRigidBody* body;

	i64 collisionGroupID;
	void* userPointer;
};

}} // Namespace end  physics::bullet