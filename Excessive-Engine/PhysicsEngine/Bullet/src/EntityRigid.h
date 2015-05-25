#pragma once
#include "../../Interfaces/IEntityRigid.h"


class btRigidBody;

class EntityRigid : public physics::IEntityRigid 
{
public:
	EntityRigid(btRigidBody* body);

	void setPos(const mm::vec3& v) override;
	void setRot(const mm::quat& q) override;
	void setScale(const mm::vec3& v) override;

	mm::vec3 getPos() override;
	mm::quat getRot() override;
	mm::vec3 getScale() override;

	btRigidBody* getBody();

protected:
	btRigidBody* body;
};