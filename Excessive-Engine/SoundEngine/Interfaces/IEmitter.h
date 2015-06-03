#pragma once

#include <mymath/mymath.h>

// namespace
namespace sound {

class ISoundData;

class IEmitter {
public:
	virtual void setPos(const mm::vec3& pos) = 0;
	virtual void setVel(const mm::vec3& vel) = 0;
	virtual void setVolume(float volume) = 0;
	virtual void setLooped(bool looped) = 0;

	virtual void start() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
	
	virtual mm::vec3 getPos() const = 0;
	virtual mm::vec3 getVel() const = 0;
	virtual float getVolume() const = 0;
	virtual bool getLooped() const = 0;

	virtual void setSoundData(ISoundData* data) = 0;
	virtual ISoundData* getSoundData()  = 0;
};

} // namespace
