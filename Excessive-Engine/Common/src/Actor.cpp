#include "Actor.h"

#include "..\GraphicsEngine\Interfaces\IEntity.h"
#include "..\PhysicsEngine\Interfaces\IEntityRigid.h"

Actor::~Actor()
{
	for (auto& a : components)
		delete a;
}

WorldComponent* Actor::addChild(WorldComponent* c)
{
	worldComponents.push_back(c);
	return c;
}

Component* Actor::addChild(Component* c)
{
	components.push_back(c);
	return c;
}

const std::vector<WorldComponent*>& Actor::getWorldComponents() const
{
	return worldComponents;
}

const std::vector<Component*>& Actor::getComponents() const
{
	return components;
}