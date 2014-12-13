#pragma once

#include "mymath\mm_common.h"
#include "mymath\mymath.h"
#include "BasicTypes.h"


enum class eProjType : u32 {
	ORTHO,
	PERSP,
};

struct rProjOrtho {
	rProjOrtho(){}
	rProjOrtho(float left, float right, float bottom, float top):left(left), right(right), top(top), bottom(bottom){}

	float left, right, top, bottom;
};

struct rProjPersp {
	rProjPersp(){}
	rProjPersp(float fovRad, float aspectRatio):fovRad(fovRad), aspectRatio(aspectRatio){}

	float fovRad;
	float aspectRatio;
};

class Camera {
public:
	void setFOV(float rad);
	void setAspectRatio(float r);
	void setNearPlane(float nP);
	void setFarPlane(float fP);

	void setPos(const mm::vec3& p);
	void setTarget(const mm::vec3& p);
	void setDir(const mm::vec3& p);

	float getFOVRad() const;
	float getAspectRatio() const;
	float getNearPlane() const;
	float getFarPlane() const;

	mm::mat4 getViewMatrix() const;
	mm::mat4 getProjMatrix() const;

	mm::vec3 getDirFront() const;
	mm::vec3 getDirBack() const;
	mm::vec3 getDirUp() const;
	mm::vec3 getDirDown() const;
	mm::vec3 getDirRight() const;
	mm::vec3 getDirLeft() const;

	const mm::vec3& getPos() const;
	const mm::quat& getRot() const;
	const mm::vec3& getTarget() const;

	Camera(rProjOrtho proj, float nearPlane, float farPlane);
	Camera(rProjPersp proj, float nearPlane, float farPlane);
	Camera();
protected:
	void calcProjMatrix();
protected:
	// Pos, Target ( World space )
	mm::vec3 pos;
	mm::vec3 target;

	// View params
	float nearPlane;
	float farPlane;

	// Projection params ( TODO: little bit stupid to prepare camera class for every projection )
	eProjType projType;
	rProjOrtho projOrtho;
	rProjPersp projPersp;
	mm::mat4 proj;
};