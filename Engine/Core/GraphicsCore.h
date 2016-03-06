#pragma once
#include "EngineCore.h"

class GraphicsCore
{
public:
	inline Window* GetTargetWindow()
	{
		return Core.GetTargetWindow();
	}

	inline void SetCam(CameraComponent* c)
	{
		Core.SetCam(c);
	}
};

extern GraphicsCore Graphics;