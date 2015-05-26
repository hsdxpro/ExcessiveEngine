#pragma once

#include "SFML/Audio.hpp"

#include "mymath/mymath.h"

class SFMLCommonSoundEmitter
{
public:
	virtual ~SFMLCommonSoundEmitter() = 0;
	virtual void setPitch(float pitch) = 0;
	//volume is real number 0 meaning muted, and 1 meaning original volume
	virtual void setVolume(float volume) = 0;
	virtual void setPosition(const mm::vec3& newPos) = 0;

	virtual void start() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
	
	virtual float getPitch() const = 0;
	virtual float getVolume() const = 0;
	virtual mm::vec3 getPosition() const = 0;
};
