#pragma once
#include "../../../Externals/include/mymath/mymath.h"
#include "../Common/src/BasicTypes.h"

// namespace
namespace graphics {

////////////////////////////////////////////////////////////////////////////////
///	Texture is a resource representing an image/bitmap.
///
////////////////////////////////////////////////////////////////////////////////

enum class eProjType : u32 {
	ORTHO,
	PERSP,
};

struct rProjOrtho {
	rProjOrtho(){}
	rProjOrtho(float left, float right, float bottom, float top) :left(left), right(right), top(top), bottom(bottom){}

	float left, right, top, bottom;
};

struct rProjPersp {
	rProjPersp(){}
	rProjPersp(float fovRad, float aspectRatio) :fovRad(fovRad), aspectRatio(aspectRatio){}

	float fovRad;
	float aspectRatio;
};

class ICamera
{
public:
	virtual void setFOV(float rad) = 0;
	virtual void setAspectRatio(float r) = 0;
	virtual void setNearPlane(float nP) = 0;
	virtual void setFarPlane(float fP) = 0;
	
	virtual void setPos(const mm::vec3& p) = 0;
	virtual void setTarget(const mm::vec3& p) = 0;
	virtual void setDir(const mm::vec3& p) = 0;
	
	virtual float getFOVRad() const = 0;
	virtual float getAspectRatio() const = 0;
	virtual float getNearPlane() const = 0;
	virtual float getFarPlane() const = 0;
	
	virtual mm::mat4 getViewMatrix() const = 0;
	virtual mm::mat4 getProjMatrix() const = 0;
	
	virtual mm::vec3 getDirFront() const = 0;
	virtual mm::vec3 getDirBack() const = 0;
	virtual mm::vec3 getDirUp() const = 0;
	virtual mm::vec3 getDirDown() const = 0;
	virtual mm::vec3 getDirRight() const = 0;
	virtual mm::vec3 getDirLeft() const = 0;
	
	virtual const mm::vec3& getPos() const = 0;
	virtual const mm::quat& getRot() const = 0;
	virtual const mm::vec3& getTargetPos() const = 0;
};



} // namespace