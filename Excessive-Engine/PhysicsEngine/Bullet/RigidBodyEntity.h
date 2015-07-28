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

	void SetGravityScale(float s) override;
	void SetTrigger(bool bTrigger) override;
	void SetCollisionGroup(i64 ID) override;

	void SetAngularFactor(float factor) override;
	void SetKinematic(bool bKinematic) override;

	void SetPos(const mm::vec3& v) override;
	void SetRot(const mm::quat& q) override;
	void SetScaleLocal(const mm::vec3& v) override;

	const mm::vec3 GetPos() const override;
	const mm::quat GetRot() const override;
	const mm::vec3 GetScaleLocal() const override;

	__inline i64 GetCollisionGroup() const override { return collisionGroupID; }
	__inline bool GetIsTrigger() const override { return body->getCollisionFlags() & btRigidBody::CF_NO_CONTACT_RESPONSE; }

	btRigidBody* GetBody();

protected:
	btRigidBody* body;

	i64 collisionGroupID;
};

}} // Namespace end  physics::bullet