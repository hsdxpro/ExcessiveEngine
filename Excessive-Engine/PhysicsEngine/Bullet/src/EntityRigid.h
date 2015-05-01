#pragma once
#include "../../Interfaces/IEntity.h"


class btRigidBody;

class EntityRigid : public physics::IEntity 
{
public:
	EntityRigid(btRigidBody* body);

	void setPos(const mm::vec3& p) override;
	void setRot(const mm::quat& r) override;
	void setScale(const mm::vec3& s) override;

	mm::vec3 getPos() override;
	mm::quat getRot() override;
	mm::vec3 getScale() override;

	btRigidBody* getBody() { return body;  }

protected:
	btRigidBody* body;
};