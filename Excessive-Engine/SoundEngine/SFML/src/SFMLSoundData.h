#pragma once

#include "SFMLCommonSoundData.h"
#include "SFML/Audio.hpp"

class SFMLSoundData : public SFMLCommonSoundData
{
public:
	SFMLSoundData(const sf::SoundBuffer& buffer);

	SFMLCommonSoundEmitter* CreateSFMLSoundSource() const override;

private:
	sf::SoundBuffer buffer;
};

