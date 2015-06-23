#pragma once
#include "WorldComponent.h"

namespace graphics { class IEntity; class ICamera; }

class ComponentCamera : public WorldComponent
{
public:
	ComponentCamera(graphics::ICamera* cam);

	void SetDirNormed(const mm::vec3& dir);

	void SetPos(const mm::vec3& v);
	void SetRot(const mm::quat& q);

	const mm::vec3 GetPos() const;
	const mm::quat GetRot() const;

	graphics::ICamera* GetCam();

protected:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	graphics::ICamera* cam;
};