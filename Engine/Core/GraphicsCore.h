#pragma once
#include "EngineCore.h"

class GraphicsCore
{
public:
	 Window* GetTargetWindow()
	{
		return Core.GetTargetWindow();
	}

	 void SetCam(CameraComponent* c)
	{
		Core.SetCam(c);
	}
};

extern GraphicsCore Graphics;