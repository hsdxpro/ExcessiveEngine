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
	void _InnerUpdatePos() override;
	void _InnerUpdateRot() override;
	void _InnerUpdateScale() override;
	void _InnerUpdateSkew() override;

protected:
	graphics::ICamera* cam;
};