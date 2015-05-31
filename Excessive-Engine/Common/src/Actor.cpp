#include "Actor.h"

#include "..\GraphicsEngine\Interfaces\IEntity.h"
#include "..\PhysicsEngine\Interfaces\IEntityRigid.h"

Actor::~Actor()
{
}

WorldComponent* Actor::addChild(WorldComponent* c)
{
	worldComponents.push_back(c);
	return c;
}

const std::vector<WorldComponent*>& Actor::getWorldComponents() const
{
	return worldComponents;
}