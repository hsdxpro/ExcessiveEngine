#pragma once

#include "InputCore.h"
#include "EngineWorld.h"

#include "GraphicsCore.h"
#include "PhysicsCore.h"
#include "SoundCore.h"
#include "NetworkCore.h"

class Script
{
public:
	virtual void Update(float deltaSeconds)	{}
};