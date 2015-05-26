#pragma once

#include "SFMLCommonSoundEmitter.h"

#include "SFML/Audio.hpp"

#include "mymath/mymath.h"

class SFMLSoundWrapper : public SFMLCommonSoundEmitter
{
public:
	SFMLSoundWrapper(const sf::SoundBuffer& buffer);

	void setPitch(float pitch) override;
	void setVolume(float volume) override;
	void setPosition(const mm::vec3& newPos) override;

	void start() override;
	void pause() override;
	void stop() override;

	float getPitch() const override;
	float getVolume() const override;
	mm::vec3 getPosition() const override;

private:
	sf::Sound sound;
};
