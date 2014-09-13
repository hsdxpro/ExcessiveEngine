#pragma once

#include "../GraphicsEngine_Interface/interface/ITexture.h"


class Texture : public ge::ITexture 
{
public:
	void release() override;
};