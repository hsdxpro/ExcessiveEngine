#pragma once
#include "../GraphicsEngine/Interfaces/IEntity.h"
#include "../PhysicsEngine/Interfaces/IEntity.h"


// The class which has graphical, physical, etc representation in the game world
// That you can transform, iteract with it.
class Entity
{
public:
	Entity(graphics::IEntity* g, physics::IEntity* p);

	void setPos(const mm::vec3& p);
protected:
	graphics::IEntity* gEntity;
	physics::IEntity* pEntity;
};