#include "SFMLMusicData.h"

#include "SFMLMusicWrapper.h"

SFMLMusicData::SFMLMusicData(const std::string& filePath) :filePath(filePath) {
}

SFMLCommonSoundEmitter* SFMLMusicData::createSFMLSoundSource() const {
	SFMLMusicWrapper* newMusic = new SFMLMusicWrapper;
	newMusic->loadFromFile(filePath);

	return newMusic;
}
