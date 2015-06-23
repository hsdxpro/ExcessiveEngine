#include "ComponentCamera.h"
#include "..\GraphicsEngine\Interfaces\ICamera.h"

ComponentCamera::ComponentCamera(graphics::ICamera* cam)
:cam(cam)
{

}

void ComponentCamera::SetDirNormed(const mm::vec3& dir)
{
	cam->SetDirNormed(dir);
	SetRot(cam->GetRot());
}

void ComponentCamera::SetPos(const mm::vec3& v)
{
	cam->SetPos(v);
	WorldComponent::SetPos(v);
}

void ComponentCamera::SetRot(const mm::quat& q)
{
	cam->SetRot(q);
	WorldComponent::SetRot(q);
}

const mm::vec3 ComponentCamera::GetPos() const
{
	return cam->GetPos();
}

const mm::quat ComponentCamera::GetRot() const
{
	return cam->GetRot();
}

graphics::ICamera* ComponentCamera::GetCam()
{
	return cam;
}

void ComponentCamera::_InnerReflectPos()
{
	cam->SetPos(worldTransform.GetPos());
}

void ComponentCamera::_InnerReflectRot()
{
	cam->SetRot(worldTransform.GetRot());
}

void ComponentCamera::_InnerReflectSkew()
{

}