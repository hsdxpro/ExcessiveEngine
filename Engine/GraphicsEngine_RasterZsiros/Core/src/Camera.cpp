// Camera.cpp implementation
#include "Camera.h"
#include "../../Core/src/common.h"


cCamera::cCamera(cCamera::tProjOrtho proj, float nearPlane, float farPlane)
:nearPlane(nearPlane), farPlane(farPlane), target(1, 0, 0), pos(0, 0, 0), projOrtho(proj), projType(cCamera::ORTHO) {
	CalcProjMatrix();
}

cCamera::cCamera(cCamera::tProjPersp proj, float nearPlane, float farPlane)
:nearPlane(nearPlane), farPlane(farPlane), target(1, 0, 0), pos(0, 0, 0), projPersp(proj), projType(cCamera::PERSP) {
	CalcProjMatrix();
}

void cCamera::SetFOV(float rad) {
	projPersp.fovRad = rad;
	CalcProjMatrix();
}

void cCamera::SetAspectRatio(float r) {
	projPersp.aspectRatio = r;
	CalcProjMatrix();
}

void cCamera::SetNearPlane(float nP) {
	nearPlane = nP;
}

void cCamera::SetFarPlane(float fP) {
	farPlane = fP;
}

void cCamera::SetPos(const Vec3& p) {
	pos = p;
}

void cCamera::SetTarget(const Vec3& p) {
	target = p;
}

void cCamera::SetDir(const Vec3& p) {
	target = pos + p;
}

float cCamera::GetFOVRad() const {
	return projPersp.fovRad;
}

float cCamera::GetAspectRatio() const {
	return projPersp.aspectRatio;
}

float cCamera::GetNearPlane() const {
	return nearPlane;
}

float cCamera::GetFarPlane() const {
	return farPlane;
}

Matrix44 cCamera::GetViewMatrix() const {
	const Vec3 up(0.0f, 0.0f, 1.0f);
	return Matrix44ViewRH(pos, target, up);
}

void cCamera::CalcProjMatrix() {
	switch (projType)
	{
	case ORTHO:
		proj = Matrix44ProjOrtographic(nearPlane, farPlane, projOrtho.left, projOrtho.right, projOrtho.bottom, projOrtho.top);
		break;
	case PERSP:
		proj = Matrix44ProjPerspective(nearPlane, farPlane, projPersp.fovRad, projPersp.aspectRatio);
		break;
	default:
		ASSERT_MSG(false, L"OMG wtf camera not ORTHO and not PERSP");
		break;
	}
}

Matrix44 cCamera::GetProjMatrix() const {
	return proj;
}

Vec3 cCamera::GetDirFront() const {
	return target - pos;
}

Vec3 cCamera::GetDirBack() const {
	return pos - target;
}

Vec3 cCamera::GetDirUp() const {
	return Cross(GetDirRight(), GetDirFront());
}

Vec3 cCamera::GetDirDown() const {
	return Cross(GetDirFront(), GetDirRight());
}

Vec3 cCamera::GetDirRight() const {
	return Cross(GetDirFront(), Vec3(0.0f, 0.0f, 1.0f));
}

Vec3 cCamera::GetDirLeft() const {
	return Cross(Vec3(0.0f, 0.0f, 1.0f), GetDirFront());
}

const Vec3& cCamera::GetPos() const {
	return pos;
}

const Quat& cCamera::GetRot() const {
	return Quat::DirToRot(target - pos, GetDirUp());
}

const Vec3& cCamera::GetTarget() const {
	return target;
}




///////////-------------- PROJECTION STRUCTS ------------------/////////////
cCamera::tProjOrtho::tProjOrtho(float left, float right, float bottom, float top)
:left(left), right(right), bottom(bottom), top(top) {
}

cCamera::tProjPersp::tProjPersp(float fovRad, float aspectRatio)
:fovRad(fovRad), aspectRatio(aspectRatio) {
}