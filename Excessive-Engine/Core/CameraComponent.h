#pragma once
#include "WorldComponent.h"
#include "GraphicsEngine\ICamera.h"

namespace graphics { class IEntity; }

class CameraComponent : public WorldComponent
{
public:
	CameraComponent(graphics::ICamera* cam);

	void SetDirNormed(const mm::vec3& dir);

	__inline void SetFOV(float angleRad)		{cam->SetFOV(angleRad);}
	__inline void SetNearPlane(float val)		{cam->SetNearPlane(val);}
	__inline void SetFarPlane(float val)		{cam->SetFarPlane(val);}
	__inline void SetTarget(const mm::vec3& p)	{cam->SetTarget(p);}

	__inline mm::vec3 GetFrontDirNormed()	{ return cam->GetFrontDirNormed(); }
	__inline mm::vec3 GetBackDirNormed()	{ return cam->GetBackDirNormed(); }
	__inline mm::vec3 GetUpDirNormed()		{ return cam->GetUpDirNormed(); }
	__inline mm::vec3 GetDownDirNormed()	{ return cam->GetDownDirNormed(); }
	__inline mm::vec3 GetRightDirNormed()	{ return cam->GetRightDirNormed(); }
	__inline mm::vec3 GetLeftDirNormed()	{ return cam->GetLeftDirNormed(); }

	graphics::ICamera* GetCam();

protected:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	graphics::ICamera* cam;
};