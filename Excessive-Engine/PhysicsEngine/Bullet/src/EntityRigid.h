#pragma once
#include "../../Interfaces/IEntityRigid.h"


class btRigidBody;

class EntityRigid : public physics::IEntityRigid 
{
public:
	EntityRigid(btRigidBody* body);

	void SetPos(const mm::vec3& v) override;
	void SetRot(const mm::quat& q) override;
	void SetScale(const mm::vec3& v) override;

	mm::vec3 GetPos() override;
	mm::quat GetRot() override;
	mm::vec3 GetScale() override;

	btRigidBody* GetBody();

protected:
	btRigidBody* body;
};