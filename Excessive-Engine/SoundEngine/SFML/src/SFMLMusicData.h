#pragma once

#include "SFML\Audio.hpp"

#include "SFMLCommonSoundData.h"

class SFMLMusicData : public SFMLCommonSoundData
{
public:
	SFMLMusicData(const std::string& filePath); 

	SFMLCommonSoundEmitter* createSFMLSoundSource() const override;
private:
	//lets use simple std::string instead of wide char string because SFML uses std::strings anyway
	std::string filePath;
};

