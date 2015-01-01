#include "Entity.h"
#include "../Module_Interfaces/GraphicsEngine/IEntity.h"


Entity::Entity(graphics::IEntity* gEntity) 
:gEntity(gEntity) {
}