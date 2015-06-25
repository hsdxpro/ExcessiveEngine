#pragma once
#include "WorldComponent.h"

namespace graphics { class IEntity; class ICamera; }

class ComponentCamera : public WorldComponent
{
public:
	ComponentCamera(graphics::ICamera* cam);

	void SetDirNormed(const mm::vec3& dir);

	graphics::ICamera* GetCam();

protected:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	graphics::ICamera* cam;
};