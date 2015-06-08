#pragma once
#include "WorldComponent.h"

namespace graphics { class IEntity; class ICamera; }

class ComponentCamera : public WorldComponent
{
public:
	ComponentCamera(graphics::ICamera* cam);

	void setDirNormed(const mm::vec3& dir);

	void setPos(const mm::vec3& v);
	void setRot(const mm::quat& q);

	const mm::vec3 getPos() const;
	const mm::quat getRot() const;

	graphics::ICamera* getCam();

protected:
	void _innerUpdatePos() override;
	void _innerUpdateRot() override;
	void _innerUpdateScale() override;

protected:
	graphics::ICamera* cam;
};