#pragma once

#include "SFMLCommonSoundData.h"
#include "SFML/Audio.hpp"

class SFMLMusicData : public SFMLCommonSoundData
{
public:
	SFMLMusicData(const std::string& filePath); 

	SFMLCommonSoundEmitter* CreateSFMLSoundSource() const override;
private:
	//lets use simple std::string instead of wide char string because SFML uses std::strings anyway
	std::string filePath;
};

