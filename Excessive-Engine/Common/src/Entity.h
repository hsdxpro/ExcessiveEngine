#pragma once
#include "../GraphicsEngine/Interfaces/IEntity.h"


// The class which has graphical, physical, etc representation in the game world
// That you can transform, iteract with it.
class Entity
{
public:
	Entity(graphics::IEntity* gEntity);

protected:
	graphics::IEntity* gEntity;
};