#pragma once
#include "../../Interfaces/IEntity.h"


class btRigidBody;

class EntityRigid : public physics::IEntity 
{
public:
	EntityRigid(btRigidBody* body);

	void setPos(const mm::vec3& p) override;
	void setRot(const mm::quat& r) override;

	mm::vec3 getPos() override;
	mm::quat getRot() override;

	btRigidBody* getBody() { return body;  }

protected:
	btRigidBody* body;
};