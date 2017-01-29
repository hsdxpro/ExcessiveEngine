// Camera.h By Zsíroskenyér Team 2013.10.13 0:14 last modified Richárd Németh
#pragma once

// Math
#include "math/Vec3.h"
#include "math/Matrix44.h"
#include "math/Quat.h"

class cCamera {
public:
	enum eProjType
	{
		ORTHO,
		PERSP,
	};

	struct tProjOrtho
	{
		public:
		tProjOrtho(){}
		tProjOrtho(float left, float right, float bottom, float top);

		float left, right, top, bottom;
	};

	struct tProjPersp
	{
		public:
		tProjPersp(){}
		tProjPersp(float fovRad, float aspectRatio);

		float fovRad;
		float aspectRatio;
	};

	void SetFOV(float rad);
	void SetAspectRatio(float r);
	void SetNearPlane(float nP);
	void SetFarPlane(float fP);

	void SetPos(const Vec3& p);
	void SetTarget(const Vec3& p);
	void SetDir(const Vec3& p);

	float GetFOVRad() const;
	float GetAspectRatio() const;
	float GetNearPlane() const;
	float GetFarPlane() const;

	Matrix44 GetViewMatrix() const;
	Matrix44 GetProjMatrix() const;

	Vec3 GetDirFront() const;
	Vec3 GetDirBack() const;
	Vec3 GetDirUp() const;
	Vec3 GetDirDown() const;
	Vec3 GetDirRight() const;
	Vec3 GetDirLeft() const;

	const Vec3& GetPos() const;
	const Quat& GetRot() const;
	const Vec3& GetTarget() const;

	cCamera(cCamera::tProjOrtho proj, float nearPlane, float farPlane);
	cCamera(cCamera::tProjPersp proj, float nearPlane, float farPlane);
protected:
	void CalcProjMatrix();
protected:
	// Pos, Target ( World space )
	Vec3 pos;
	Vec3 target;

	// View params
	float nearPlane;
	float farPlane;

	// Projection params ( TODO: little bit stupid to prepare camera class for every projection )
	eProjType projType;
	tProjOrtho projOrtho;
	tProjPersp projPersp;
	Matrix44 proj;
};