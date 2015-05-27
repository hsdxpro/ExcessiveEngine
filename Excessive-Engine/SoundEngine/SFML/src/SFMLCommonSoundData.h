#pragma once

#include "SFML\Audio.hpp"

class SFMLCommonSoundEmitter;

class SFMLCommonSoundData
{
public:
	virtual ~SFMLCommonSoundData();

	virtual SFMLCommonSoundEmitter* createSFMLSoundSource() const = 0;
};
