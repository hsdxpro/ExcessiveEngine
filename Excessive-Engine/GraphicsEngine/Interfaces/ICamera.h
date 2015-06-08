#pragma once
#include "../../../Externals/include/mymath/mymath.h"
#include "../Common/src/BasicTypes.h"

// namespace
namespace graphics {

////////////////////////////////////////////////////////////////////////////////
///	Texture is a resource representing an image/bitmap.
///
////////////////////////////////////////////////////////////////////////////////

enum class eProjType : u32 
{
	ORTHO,
	PERSP,
};

struct rProjOrtho 
{
	rProjOrtho(){}
	rProjOrtho(float left, float right, float bottom, float top) :left(left), right(right), top(top), bottom(bottom){}

	float left, right, top, bottom;
};

struct rProjPersp
{
	rProjPersp():fovRad(3.14159265358979f /2){}
	rProjPersp(float fovRad) :fovRad(fovRad){}

	float fovRad;
};

class ICamera
{
public:
	virtual void setFOV(float rad) = 0;
	virtual void setNearPlane(float nP) = 0;
	virtual void setFarPlane(float fP) = 0;
	
	virtual void setPos(const mm::vec3& p) = 0;
	virtual void setTarget(const mm::vec3& p) = 0;
	virtual void setDirNormed(const mm::vec3& p) = 0;
	virtual void setRot(const mm::quat& q) = 0;
	
	virtual float getFOVRad() const = 0;
	virtual float getNearPlane() const = 0;
	virtual float getFarPlane() const = 0;
	
	virtual mm::mat4 getViewMatrix() const = 0;
	virtual mm::mat4 getProjMatrix(float aspectRatio) const = 0;
	
	virtual mm::vec3 getDirNormedFront() const = 0;
	virtual mm::vec3 getDirNormedBack() const = 0;
	virtual mm::vec3 getDirNormedUp() const = 0;
	virtual mm::vec3 getDirNormedDown() const = 0;
	virtual mm::vec3 getDirNormedRight() const = 0;
	virtual mm::vec3 getDirNormedLeft() const = 0;
	
	virtual const mm::vec3& getPos() const = 0;
	virtual const mm::quat& getRot() const = 0;
	virtual const mm::vec3 getTargetPos() const = 0;
};



} // namespace