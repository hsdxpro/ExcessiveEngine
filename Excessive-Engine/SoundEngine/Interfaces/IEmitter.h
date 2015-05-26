#pragma once

#include <mymath/mymath.h>

// namespace
namespace sound {

class ISoundData;

class IEmitter {
public:
	virtual void setPos(const mm::vec3&) = 0;
	virtual void setVel(const mm::vec3&) = 0;

	virtual void start() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
	
	virtual mm::vec3 getPos() const = 0;
	virtual mm::vec3 getVel() const = 0;

	virtual void setSoundData(ISoundData* data) = 0;
	virtual ISoundData* getSoundData() const = 0;
};

} // namespace
