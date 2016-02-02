#pragma once

#include "SFML/Audio.hpp"

#include "mymath/mymath.h"

class SFMLCommonSoundEmitter
{
public:
	virtual ~SFMLCommonSoundEmitter();
	virtual void SetPitch(float pitch) = 0;
	//volume is real number 0 meaning muted, and 1 meaning original volume
	virtual void SetVolume(float volume) = 0;
	virtual void SetPosition(const mm::vec3& newPos) = 0;
	virtual void SetLooped(bool loop) = 0;

	virtual void Start() = 0;
	virtual void Pause() = 0;
	virtual void Stop() = 0;
	
	virtual float GetPitch() const = 0;
	virtual float GetVolume() const = 0;
	virtual mm::vec3 GetPosition() const = 0;
	virtual bool GetLooped() const = 0;
};
