#include "CameraComponent.h"

CameraComponent::CameraComponent(ICamera* cam)
:cam(cam)
{

}

void CameraComponent::SetDirNormed(const mm::vec3& dir)
{
	cam->SetDirNormed(dir);
	SetRot(cam->GetRot());
}

ICamera* CameraComponent::GetCam()
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