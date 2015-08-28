#pragma once
#include "EngineCore.h"

class GraphicsCore
{
public:
	__inline Window* GetTargetWindow()
	{
		return Core.GetTargetWindow();
	}

	__inline void SetCam(CameraComponent* c)
	{
		Core.SetCam(c);
	}
};

extern GraphicsCore Graphics;