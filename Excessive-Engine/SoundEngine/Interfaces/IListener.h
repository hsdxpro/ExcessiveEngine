#pragma once

#include <mymath/mymath.h>

// namespace
namespace sound {

class IListener {
public:
	virtual void setPos(mm::vec3&) = 0;
	virtual void setVel(mm::vec3&) = 0;
	
	virtual mm::vec3 getPos() const = 0;
	virtual mm::vec3 getVel() const = 0;
};
	
} // namespace
