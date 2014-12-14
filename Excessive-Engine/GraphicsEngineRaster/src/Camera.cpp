// Camera.cpp implementation
#include "Camera.h"
#include <cassert>
#include "mymath\mm_quat_func.h"

using namespace mymath;

Camera::Camera(graphics::rProjOrtho proj, float nearPlane, float farPlane)
:nearPlane(nearPlane), farPlane(farPlane), target(0, 0, 1), pos(0, 0, 0), projOrtho(proj), projType(graphics::eProjType::ORTHO) {
	calcProjMatrix();
}

Camera::Camera(graphics::rProjPersp proj, float nearPlane, float farPlane)
: nearPlane(nearPlane), farPlane(farPlane), target(0, 0, 1), pos(0, 0, 0), projPersp(proj), projType(graphics::eProjType::PERSP) {
	calcProjMatrix();
}

Camera::Camera()
: nearPlane(0), farPlane(0), target(0, 0, 1), pos(0, 0, 0), projType(graphics::eProjType::PERSP) {
}

void Camera::setFOV(float rad) {
	assert(projType == graphics::eProjType::PERSP);

	projPersp.fovRad = rad;
	calcProjMatrix();
}

void Camera::setAspectRatio(float r) {
	assert(projType == graphics::eProjType::PERSP);

	projPersp.aspectRatio = r;
	calcProjMatrix();
}

void Camera::setNearPlane(float nP) {
	nearPlane = nP;
	calcProjMatrix();
}

void Camera::setFarPlane(float fP) {
	farPlane = fP;
	calcProjMatrix();
}

void Camera::setPos(const mm::vec3& p) {
	mm::vec3 delta = pos - target;

	pos = p;
	target = p + delta;
}

void Camera::setTarget(const mm::vec3& p) {
	target = p;
}

void Camera::setDir(const mm::vec3& p) {
	target = pos + p;
}

float Camera::getFOVRad() const {
	return projPersp.fovRad;
}

float Camera::getAspectRatio() const {
	return projPersp.aspectRatio;
}

float Camera::getNearPlane() const {
	return nearPlane;
}

float Camera::getFarPlane() const {
	return farPlane;
}

// TODO REMOVE IT OR I KILL MYSELF
mm::mat4 Matrix44ViewRH(const mm::vec3& eye, const mm::vec3& target, const mm::vec3& up) {
	mm::vec3 baseFront = normalize(target - eye);			// The "look-at" vector.
	mm::vec3 baseRight = normalize(cross(baseFront, up));	// The "right" vector.
	mm::vec3 baseUp = cross(baseRight, baseFront);			// The "up" vector.
	
	// Create a 4x4 orientation matrix from the right, up, and at vectors
	// TRANPOSE of ROT
	mm::mat4 orientation(baseRight.x, baseUp.x, baseFront.x, 0,
		baseRight.y, baseUp.y, baseFront.y, 0,
		baseRight.z, baseUp.z, baseFront.z, 0,
		0, 0, 0, 1);

	// Create a 4x4 translation matrix by negating the eye position.
	// NEGATE POS
	mm::mat4 translation(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-eye.x, -eye.y, -eye.z, 1);

	return translation;	
}

mm::mat4 Camera::getViewMatrix() const {
	const mm::vec3 up(0.0f, 1.0f, 0.0f);
	return Matrix44ViewRH(pos, target, up);
}

void Camera::calcProjMatrix() {
	switch (projType)
	{
	case graphics::eProjType::ORTHO:
		proj = ortographic(projOrtho.left, projOrtho.right, projOrtho.bottom, projOrtho.top, nearPlane, farPlane);
		break;
	case graphics::eProjType::PERSP:
		proj = perspective(projPersp.fovRad, projPersp.aspectRatio, nearPlane, farPlane);
		break;
	default:
		assert(0);
		break;
	}
}

mm::mat4 Camera::getProjMatrix() const {
	return proj;
}

mm::vec3 Camera::getDirFront() const {
	return target - pos;
}

mm::vec3 Camera::getDirBack() const {
	return pos - target;
}

mm::vec3 Camera::getDirUp() const {
	return cross(getDirRight(), getDirFront());
}

mm::vec3 Camera::getDirDown() const {
	return cross(getDirFront(), getDirRight());
}

mm::vec3 Camera::getDirRight() const {
	return cross(getDirFront(), mm::vec3(0.0f, 0.0f, 1.0f));
}

mm::vec3 Camera::getDirLeft() const {
	return cross(mm::vec3(0.0f, 0.0f, 1.0f), getDirFront());
}

const mm::vec3& Camera::getPos() const {
	return pos;
}

const mm::quat& Camera::getRot() const {
	// TODO
	return mm::quat();// ::DirToRot(target - pos, GetDirUp());
}

const mm::vec3& Camera::getTarget() const {
	return target;
}