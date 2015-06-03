#pragma once

#include "SFMLCommonSoundData.h"
#include "SFML/Audio.hpp"

class SFMLSoundData : public SFMLCommonSoundData
{
public:
	SFMLSoundData(const sf::SoundBuffer& buffer);

	SFMLCommonSoundEmitter* createSFMLSoundSource() const override;

private:
	sf::SoundBuffer buffer;
};

