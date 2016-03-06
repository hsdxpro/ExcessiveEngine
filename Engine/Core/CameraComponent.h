#pragma once

#include "WorldComponent.h"
#include "SupportLibrary\ICamera.h"

namespace graphics { class IEntity; }

class CameraComponent : public WorldComponent
{
public:
	static const eWorldComponentType TYPE = CAMERA;

public:
	inline CameraComponent(ICamera* cam);

	inline void SetDirNormed(const mm::vec3& dir);

	inline void SetFOV(float angleRad);
	inline void SetNearPlane(float val);
	inline void SetFarPlane(float val);
	inline void SetTarget(const mm::vec3& p);

	inline mm::vec3 GetFrontDir();
	inline mm::vec3 GetBackDir();
	inline mm::vec3 GetUpDir();
	inline mm::vec3 GetDownDir();
	inline mm::vec3 GetRightDir();
	inline mm::vec3 GetLeftDir();

	inline ICamera* GetCam();

protected:
	ICamera* cam;
};

CameraComponent::CameraComponent(ICamera* cam)
:WorldComponent(TYPE), cam(cam)
{
	
}

void CameraComponent::SetDirNormed(const mm::vec3& dir)
{
	cam->SetDirNormed(dir);
	SetRot(cam->GetRot());
}

void CameraComponent::SetFOV(float angleRad)
{
	cam->SetFOV(angleRad);
}

void CameraComponent::SetNearPlane(float val)
{
	cam->SetNearPlane(val);
}

void CameraComponent::SetFarPlane(float val)
{
	cam->SetFarPlane(val);
}

void CameraComponent::SetTarget(const mm::vec3& p)
{
	cam->SetTarget(p);
}

mm::vec3 CameraComponent::GetFrontDir()
{
	return cam->GetFrontDir();
}

mm::vec3 CameraComponent::GetBackDir()
{
	return cam->GetBackDir();
}

mm::vec3 CameraComponent::GetUpDir()
{
	return cam->GetUpDir();
}

mm::vec3 CameraComponent::GetDownDir()
{
	return cam->GetDownDir();
}

mm::vec3 CameraComponent::GetRightDir()
{
	return cam->GetRightDir();
}

mm::vec3 CameraComponent::GetLeftDir()
{
	return cam->GetLeftDir();
}

ICamera* CameraComponent::GetCam()
{
	return cam;
}