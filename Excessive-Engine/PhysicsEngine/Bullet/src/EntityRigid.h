#pragma once
#include "../../Interfaces/IEntity.h"

class btRigidBody;

class EntityRigid : public physics::IEntity 
{
public:
	EntityRigid(btRigidBody* body);
	btRigidBody* getBody() { return body;  }

protected:
	btRigidBody* body;
};