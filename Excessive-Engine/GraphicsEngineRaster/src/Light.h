#pragma once

#include "../GraphicsEngine_Interface/interface/ILight.h"


class Light : public graphics::ILight
{
public:
	void setColor() override;
	void setPosition() override;
	void setDirection() override;
};