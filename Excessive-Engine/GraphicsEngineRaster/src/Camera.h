#pragma once
#include "../Module_Interfaces/GraphicsEngine/ICamera.h"

#include "mymath/mm_common.h"
#include "mymath/mymath.h"
#include "../../Common/src/BasicTypes.h"

class Camera : public graphics::ICamera 
{
public:
	void setFOV(float rad) override;
	void setAspectRatio(float r) override;
	void setNearPlane(float nP) override;
	void setFarPlane(float fP) override;

	void setPos(const mm::vec3& p) override;
	void setTarget(const mm::vec3& p) override;
	void setDir(const mm::vec3& p) override;

	float getFOVRad() const override;
	float getAspectRatio() const override;
	float getNearPlane() const override;
	float getFarPlane() const override;

	mm::mat4 getViewMatrix() const override;
	mm::mat4 getProjMatrix() const override;

	mm::vec3 getDirFront() const override;
	mm::vec3 getDirBack() const override;
	mm::vec3 getDirUp() const override;
	mm::vec3 getDirDown() const override;
	mm::vec3 getDirRight() const override;
	mm::vec3 getDirLeft() const override;

	const mm::vec3& getPos() const override;
	const mm::quat& getRot() const override;
	const mm::vec3& getTarget() const override;

	Camera(graphics::rProjOrtho proj, float nearPlane, float farPlane);
	Camera(graphics::rProjPersp proj, float nearPlane, float farPlane);
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
	graphics::eProjType projType;
	graphics::rProjOrtho projOrtho;
	graphics::rProjPersp projPersp;
	mm::mat4 proj;
};