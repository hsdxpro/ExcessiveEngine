#include "SFMLSoundData.h"

#include "SFMLSoundWrapper.h"

SFMLSoundData::SFMLSoundData(const sf::SoundBuffer& buffer) : buffer(buffer) {
}

SFMLCommonSoundEmitter* SFMLSoundData::createSFMLSoundSource() const {
	return new SFMLSoundWrapper(buffer);
}
