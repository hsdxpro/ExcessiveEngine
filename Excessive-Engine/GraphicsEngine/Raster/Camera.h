#pragma once
#include "../ICamera.h"

#include "mymath/mm_common.h"
#include "mymath/mymath.h"

class Camera : public graphics::ICamera 
{
public:
	void SetFOV(float rad) override;
	void SetNearPlane(float nP) override;
	void SetFarPlane(float fP) override;

	void SetPos(const mm::vec3& p) override;
	void SetTarget(const mm::vec3& p) override;
	void SetDirNormed(const mm::vec3& p) override;
	void SetRot(const mm::quat& q) override;

	float GetFOVRad() const override;
	float GetNearPlane() const override;
	float GetFarPlane() const override;

	mm::mat4 GetViewMatrix() const override;
	mm::mat4 GetProjMatrix(float aspectRatio) const override;

	mm::vec3 GetDirFrontNormed() const override;
	mm::vec3 GetDirBackNormed() const override;
	mm::vec3 GetDirUpNormed() const override;
	mm::vec3 GetDirDownNormed() const override;
	mm::vec3 GetDirRightNormed() const override;
	mm::vec3 GetDirLeftNormed() const override;

	const mm::vec3& GetPos() const override;
	const mm::quat& GetRot() const override;
	const mm::vec3 GetTargetPos() const override;

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