#pragma once

#include "../../Interfaces/ICamera.h"

#include "mymath/mm_common.h"
#include "mymath/mymath.h"

class Camera : public graphics::ICamera 
{
public:
	void setFOV(float rad) override;
	void setNearPlane(float nP) override;
	void setFarPlane(float fP) override;

	void setPos(const mm::vec3& p) override;
	void setTarget(const mm::vec3& p) override;
	void setDirNormed(const mm::vec3& p) override;
	void setRot(const mm::quat& q) override;

	float getFOVRad() const override;
	float getNearPlane() const override;
	float getFarPlane() const override;

	mm::mat4 getViewMatrix() const override;
	mm::mat4 getProjMatrix(float aspectRatio) const override;

	mm::vec3 getDirNormedFront() const override;
	mm::vec3 getDirNormedBack() const override;
	mm::vec3 getDirNormedUp() const override;
	mm::vec3 getDirNormedDown() const override;
	mm::vec3 getDirNormedRight() const override;
	mm::vec3 getDirNormedLeft() const override;

	const mm::vec3& getPos() const override;
	const mm::quat& getRot() const override;
	const mm::vec3 getTargetPos() const override;

	Camera(graphics::rProjOrtho proj, float nearPlane, float farPlane);
	Camera(graphics::rProjPersp proj, float nearPlane, float farPlane);
	Camera();

//protected:
//	void calcProjMatrix();

protected:
	// Pos, Target ( World space )
	mm::vec3 pos;
	mm::quat rot;

	// View params
	float nearPlane;
	float farPlane;

	// Projection params ( TODO: little bit stupid to prepare camera class for every projection )
	graphics::eProjType projType;
	graphics::rProjOrtho projOrtho;
	graphics::rProjPersp projPersp;
	mm::mat4 proj;
};