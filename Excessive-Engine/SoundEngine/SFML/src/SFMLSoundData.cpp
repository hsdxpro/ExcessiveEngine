#include "SFMLSoundData.h"

#include "SFMLSoundWrapper.h"

SFMLSoundData::SFMLSoundData(const sf::SoundBuffer& buffer) : buffer(buffer) {
}

SFMLCommonSoundEmitter* SFMLSoundData::CreateSFMLSoundSource() const {
	return new SFMLSoundWrapper(buffer);
}
