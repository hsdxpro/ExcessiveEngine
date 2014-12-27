#include "Entity.h"

#include "..\GraphicsEngine_Interface\interface\IEntity.h"

Entity::Entity(graphics::IEntity* gEntity) 
:gEntity(gEntity) {
}