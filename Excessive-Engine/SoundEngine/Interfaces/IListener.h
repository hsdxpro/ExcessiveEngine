#pragma once

#include "../Common/src/BasicTypes.h"
#include <mymath/mymath.h>

// namespace
namespace sound {

enum class eHandedness : u32 {
	LEFT,
	RIGHT,
};

class IListener {
public:
	
	virtual void setHandedness(eHandedness handedness) = 0;
	virtual void setUpwards(const mm::vec3&) = 0;
	virtual void setTarget(const mm::vec3&) = 0;
	virtual void setDir(const mm::vec3&) = 0;
	virtual void setPos(const mm::vec3&) = 0;
	virtual void setVel(const mm::vec3&) = 0;
	
	virtual eHandedness getHandedness() const = 0;
	virtual mm::vec3 getUpwards() const = 0;
	virtual mm::vec3 getDir() const = 0;
	virtual mm::vec3 getPos() const = 0;
	virtual mm::vec3 getVel() const = 0;
};

} // namespace
