#include "ComponentGraphics.h"
#include "..\GraphicsEngine\Interfaces\IEntity.h"

void ComponentGraphics::setPos(const mm::vec3& v)
{
	entityGraphics->setPos(v);
	WorldComponent::setPos(v);
}

void ComponentGraphics::setRot(const mm::quat& q)
{
	entityGraphics->setRot(q);
	WorldComponent::setRot(q);
}
void ComponentGraphics::setScale(const mm::vec3& v)
{
	entityGraphics->setScale(v);
	WorldComponent::setScale(v);
}

const mm::vec3& ComponentGraphics::getPos()
{
	return entityGraphics->getPos();
}

const mm::quat& ComponentGraphics::getRot()
{
	return entityGraphics->getRot();
}

const mm::vec3& ComponentGraphics::getScale()
{
	return entityGraphics->getScale();
}