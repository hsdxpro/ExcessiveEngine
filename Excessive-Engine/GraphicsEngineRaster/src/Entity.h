#pragma once

#include "../GraphicsEngine_Interface/interface/IEntity.h"


class Entity : ge::IEntity
{
public:
	void release() override;

	void setPosition() override;
	void setRotation() override;
	void setScale() override;

	void setMaterial(ge::IMaterial* material) override;
};