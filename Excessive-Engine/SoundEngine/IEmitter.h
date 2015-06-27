#pragma once

#include <mymath/mymath.h>

// namespace
namespace sound {

class ISoundData;

class IEmitter {
public:
	virtual void SetPos(const mm::vec3& pos) = 0;
	virtual void SetVel(const mm::vec3& vel) = 0;
	virtual void SetVolume(float volume) = 0;
	virtual void SetLooped(bool looped) = 0;

	virtual void Start() = 0;
	virtual void Pause() = 0;
	virtual void Stop() = 0;
	
	virtual mm::vec3 GetPos() const = 0;
	virtual mm::vec3 GetVel() const = 0;
	virtual float GetVolume() const = 0;
	virtual bool GetLooped() const = 0;

	virtual void SetSoundData(ISoundData* data) = 0;
	virtual ISoundData* GetSoundData()  = 0;
};

} // namespace
