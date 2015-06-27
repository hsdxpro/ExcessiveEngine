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

graphics::ICamera* ComponentCamera::GetCam()
{
	return cam;
}

void ComponentCamera::_InnerReflectPos()
{
	cam->SetPos(transform.GetPos());
}

void ComponentCamera::_InnerReflectRot()
{
	cam->SetRot(transform.GetRot());
}

void ComponentCamera::_InnerReflectSkew()
{

}