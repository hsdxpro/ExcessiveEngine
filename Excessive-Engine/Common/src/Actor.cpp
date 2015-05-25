#include "Actor.h"

#include "..\GraphicsEngine\Interfaces\IEntity.h"
#include "..\PhysicsEngine\Interfaces\IEntityRigid.h"

Actor::Actor()
{
}

//void Actor::setPos(const mm::vec3& p)
//{
//	// TEMPORARY CODE !!! REMOVE
//	for (auto& rigid : componentTypeContainer[(u32)eCompType::RIGID].childComponents)
//		rigid->setPos(p);
//
//	for (auto& rigid : componentTypeContainer[(u32)eCompType::RIGID].childComponents)
//		rigid->setPos(p);
//}
//
//void Actor::setScale(const mm::vec3& s)
//{
//	// TEMPORARY CODE !!! REMOVE
//	for (auto& rigid : childComponents->componentsRigid)
//		rigid->setScale(s);
//
//	for (auto& rigid : childComponents->componentsGraphics)
//		rigid->setScale(s);
//}