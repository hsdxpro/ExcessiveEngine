#include "SFMLMusicData.h"

#include "SFMLMusicWrapper.h"

SFMLMusicData::SFMLMusicData(const std::string& filePath) :filePath(filePath) {
}

SFMLCommonSoundEmitter* SFMLMusicData::CreateSFMLSoundSource() const {
	SFMLMusicWrapper* newMusic = new SFMLMusicWrapper;
	newMusic->LoadFromFile(filePath);

	return newMusic;
}
