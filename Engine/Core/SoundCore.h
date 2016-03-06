#pragma once
#include "EngineCore.h"

class SoundCore
{
public:
	inline sound::IEmitter* CreateMonoSound(const std::string& filePath, float volumeNormedPercent = 1, bool bLoop = false)
	{
		return Core.CreateMonoSound(filePath, volumeNormedPercent, bLoop);
	}
};

extern SoundCore Sound;