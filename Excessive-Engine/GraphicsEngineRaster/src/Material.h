#pragma once

#include "../GraphicsEngine_Interface/interface/IMaterial.h"


class Material : ge::IMaterial
{
public:
	void release() override;
	

};