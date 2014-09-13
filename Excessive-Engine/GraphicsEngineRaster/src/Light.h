#pragma once

#include "../GraphicsEngine_Interface/interface/ILight.h"


class Light : public ge::ILight
{
public:
	void release() override;

	void setColor() override;
	void setPosition() override;
	void setDirection() override;
};