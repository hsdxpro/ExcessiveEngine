#pragma once
// The class which has graphical, physical, etc representation in the game world
// That you can transform, iteract with it.

#include "..\GraphicsEngine_Interface\interface\IEntity.h"

class Entity
{
public:
	Entity(graphics::IEntity* gEntity);

protected:
	graphics::IEntity* gEntity;
};