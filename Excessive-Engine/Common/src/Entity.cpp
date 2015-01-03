#include "Entity.h"

Entity::Entity(graphics::IEntity* g, physics::IEntity* p)
:gEntity(g), pEntity(p) {
}

void Entity::setPos(const mm::vec3& p) {
	gEntity->setPos(p);
	//pEntity->setPos(p);
}