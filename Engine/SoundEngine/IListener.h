#pragma once
#include "BaseLibrary/Types.h"
#include <mymath/mymath.h>


// namespace
namespace sound {

//TODO move handedness to engine class
enum class eHandedness : u32 {
	LEFT,
	RIGHT,
};

class IListener {
public:
	
	virtual void SetHandedness(eHandedness handedness) = 0;
	virtual void SetUpwards(const mm::vec3&) = 0;
	virtual void SetTarget(const mm::vec3&) = 0;
	virtual void SetDir(const mm::vec3&) = 0;
	virtual void SetPos(const mm::vec3&) = 0;
	virtual void SetVel(const mm::vec3&) = 0;
	
	virtual eHandedness GetHandedness() const = 0;
	virtual mm::vec3 GetUpwards() const = 0;
	virtual mm::vec3 GetDir() const = 0;
	virtual mm::vec3 GetPos() const = 0;
	virtual mm::vec3 GetVel() const = 0;
};

} // namespace
