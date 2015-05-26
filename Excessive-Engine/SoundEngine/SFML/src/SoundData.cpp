#include "SoundData.h"

#include "SFMLMusicData.h"
#include "SFMLSoundData.h"

SoundData::SoundData() : refcount(1), pSFMLSoundData(nullptr) {
}

SoundData::~SoundData() {
	delete pSFMLSoundData;
}

void SoundData::acquire() {
	refcount++;
}

void SoundData::release() {
	refcount--;
	if (refcount == 0) {
		delete this;
	}
}

bool SoundData::load(const wchar_t* file_path, sound::StoreMode mode) {
	std::string narrow_file_path;
	//FIXME probably a bad way to convert wide string to standard
	for (int i = 0; file_path[i] != L'\0'; i++) {
		narrow_file_path += (char)(file_path[i]);
	}

	if (mode == sound::StoreMode::AUTOMATIC) {
		//determine audio length
		sf::Music audioFile;
		
		if (!audioFile.openFromFile(narrow_file_path)) {
			return false;
		}

		if (audioFile.getDuration().asSeconds() > 5) {
			mode = sound::StoreMode::STREAMED;
		} else {
			mode = sound::StoreMode::BUFFERED;
		}
	}

	delete pSFMLSoundData;

	switch (mode) {
	case sound::StoreMode::STREAMED: {
		pSFMLSoundData = new SFMLMusicData(narrow_file_path);
		break;
	}

	case sound::StoreMode::BUFFERED: {
		sf::SoundBuffer buffer;
		buffer.loadFromFile(narrow_file_path);
		pSFMLSoundData = new SFMLSoundData(buffer);
		break;
	}

	default:
		break;
	}

	return true;
}

const SFMLCommonSoundData* SoundData::getSFMLSoundData() const {
	return pSFMLSoundData;
}
