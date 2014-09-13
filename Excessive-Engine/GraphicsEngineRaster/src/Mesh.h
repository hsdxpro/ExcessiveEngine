#pragma once

#include "../GraphicsEngine_Interface/interface/IMesh.h"


class Mesh : public ge::IMesh
{
public:
	void release() override;
};