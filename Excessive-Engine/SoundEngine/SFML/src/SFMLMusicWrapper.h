#pragma once

#include "SFMLCommonSoundEmitter.h"

#include "SFML/Audio.hpp"
#include "mymath/mymath.h"

class SFMLMusicWrapper : public SFMLCommonSoundEmitter
{
public:
	SFMLMusicWrapper();
	bool loadFromFile(const std::string& filename);

	void setPitch(float pitch) override;
	void setVolume(float volume) override;
	void setPosition(const mm::vec3& newPos) override;
	void setLooped(bool looped) override;
	
	void start() override;
	void pause() override;
	void stop() override;

	float getPitch() const override;
	float getVolume() const override;
	mm::vec3 getPosition() const override;
	bool getLooped() const override;

private:
	sf::Music music;
};
