#pragma once
#include "WorldComponent.h"
#include "GraphicsEngine\ICamera.h"

namespace graphics { class IEntity; }

class ComponentCamera : public WorldComponent
{
public:
	ComponentCamera(graphics::ICamera* cam);

	void SetDirNormed(const mm::vec3& dir);

	__inline void SetFOV(float angleRad)		{cam->SetFOV(angleRad);}
	__inline void SetNearPlane(float val)		{cam->SetNearPlane(val);}
	__inline void SetFarPlane(float val)		{cam->SetFarPlane(val);}
	__inline void SetTarget(const mm::vec3& p)	{cam->SetTarget(p);}

	graphics::ICamera* GetCam();

protected:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	graphics::ICamera* cam;
};