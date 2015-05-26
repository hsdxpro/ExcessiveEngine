#pragma once

#include "SFML\Audio.hpp"

#include "SFMLCommonSoundEmitter.h"

class SFMLCommonSoundData
{
public:
	virtual ~SFMLCommonSoundData() = 0;

	virtual SFMLCommonSoundEmitter* createSFMLSoundSource() const = 0;
};
