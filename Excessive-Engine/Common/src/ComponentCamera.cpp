#include "ComponentCamera.h"
#include "..\GraphicsEngine\Interfaces\ICamera.h"

ComponentCamera::ComponentCamera(graphics::ICamera* cam)
:cam(cam)
{

}

void ComponentCamera::setDirNormed(const mm::vec3& dir)
{
	cam->setDirNormed(dir);
	setRot(cam->getRot());
}

void ComponentCamera::setPos(const mm::vec3& v)
{
	cam->setPos(v);
	WorldComponent::setPos(v);
}

void ComponentCamera::setRot(const mm::quat& q)
{
	cam->setRot(q);
	WorldComponent::setRot(q);
}

const mm::vec3 ComponentCamera::getPos() const
{
	return cam->getPos();
}

const mm::quat ComponentCamera::getRot() const
{
	return cam->getRot();
}

graphics::ICamera* ComponentCamera::getCam()
{
	return cam;
}

void ComponentCamera::_innerUpdatePos()
{
	cam->setPos(worldTransform.getPos());
}

void ComponentCamera::_innerUpdateRot()
{
	cam->setRot(worldTransform.getRot());
}

void ComponentCamera::_innerUpdateScale()
{

}