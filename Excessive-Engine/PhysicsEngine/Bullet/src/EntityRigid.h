#pragma once
#include "../../Interfaces/IEntityRigid.h"


class btRigidBody;

class EntityRigid : public physics::IEntityRigid 
{
public:
	EntityRigid(btRigidBody* body);

	void updateAfterSimulate();

	void setPos(const mm::vec3& v) override;
	void setRot(const mm::quat& q) override;
	void setScale(const mm::vec3& v) override;

	const mm::vec3& getPos() override;
	const mm::quat& getRot() override;
	const mm::vec3& getScale() override;

	btRigidBody* getBody() { return body;  }

protected:
	btRigidBody* body;
};