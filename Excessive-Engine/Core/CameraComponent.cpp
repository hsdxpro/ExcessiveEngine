#include "CameraComponent.h"

CameraComponent::CameraComponent(graphics::ICamera* cam)
:cam(cam)
{

}

void CameraComponent::SetDirNormed(const mm::vec3& dir)
{
	cam->SetDirNormed(dir);
	SetRot(cam->GetRot());
}

graphics::ICamera* CameraComponent::GetCam()
{
	return cam;
}

void CameraComponent::_InnerReflectPos()
{
	cam->SetPos(transform.GetPos());
}

void CameraComponent::_InnerReflectRot()
{
	cam->SetRot(transform.GetRot());
}

void CameraComponent::_InnerReflectSkew()
{

}