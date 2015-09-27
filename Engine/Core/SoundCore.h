#pragma once
#include "EngineCore.h"

class SoundCore
{
public:
	__inline sound::IEmitter* CreateSoundMono(const std::string& filePath, float volumeNormedPercent = 1, bool bLoop = false)
	{
		return Core.CreateSoundMono(filePath, volumeNormedPercent, bLoop);
	}
};

extern SoundCore Sound;