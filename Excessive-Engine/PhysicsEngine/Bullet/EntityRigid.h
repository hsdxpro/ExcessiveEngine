#pragma once
#include "../../Interfaces/IEntityRigid.h"


class btRigidBody;

class EntityRigid : public physics::IEntityRigid 
{
public:
	EntityRigid(btRigidBody* body);

	void AddForce(const mm::vec3& force, const mm::vec3& relPos = {0,0,0}) override;
	
	void SetAngularFactor(float factor) override;
	void SetKinematic() override;

	void SetPos(const mm::vec3& v) override;
	void SetRot(const mm::quat& q) override;
	void SetScaleLocal(const mm::vec3& v) override;

	const mm::vec3 GetPos() const override;
	const mm::quat GetRot() const override;
	const mm::vec3 GetScaleLocal() const override;

	btRigidBody* GetBody();

protected:
	btRigidBody* body;
};