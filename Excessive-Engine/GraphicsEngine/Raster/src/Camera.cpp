// Camera.cpp implementation
#include "Camera.h"

#include <cassert>
#include "mymath/mm_quat_func.h"

// TODO REMOVE IT OR I KILL MYSELF
mm::mat4 Matrix44ViewRHGL(const mm::vec3& eye, const mm::vec3& target, const mm::vec3& up)
{
	// Negate cuz of OpenGL -z front...
	mm::vec3 baseFront = mymath::normalize(-(target - eye));		// The "look-at" vector.
	mm::vec3 baseRight = mymath::normalize(mymath::cross(up, baseFront));	// The "right" vector.
	mm::vec3 baseUp = mymath::cross(baseFront, baseRight);			// The "up" vector.

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

	return orientation * translation;
}

mm::mat4 Matrix44ViewRH(const mm::vec3& eye, const mm::vec3& target, const mm::vec3& up)
{
	// Negate cuz of OpenGL -z front...
	mm::vec3 baseFront = mymath::normalize(target - eye);		// The "look-at" vector.
	mm::vec3 baseRight = mymath::normalize(mymath::cross(up, baseFront));	// The "right" vector.
	mm::vec3 baseUp = mymath::cross(baseFront, baseRight);			// The "up" vector.

	// Create a 4x4 orientation matrix from the right, up, and at vectors
	// TRANPOSE of ROT
	mm::mat4 orientation(baseRight.x, baseFront.x, baseUp.x, 0,
						 baseRight.y, baseFront.y, baseUp.y, 0,
						 baseRight.z, baseFront.z, baseUp.z, 0,
						 0, 0, 0, 1);

	// Create a 4x4 translation matrix by negating the eye position.
	// NEGATE POS
	mm::mat4 translation(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-eye.x, -eye.y, -eye.z, 1);

	return orientation * translation;
}

Camera::Camera()
:nearPlane(0.01), farPlane(4000), pos(0, 0, 0), projType(graphics::eProjType::PERSP)
{
	setDirNormed({ 0, 1, 0 });
}

Camera::Camera(graphics::rProjOrtho proj, float nearPlane, float farPlane)
:nearPlane(nearPlane), farPlane(farPlane), pos(0, 0, 0), projOrtho(proj), projType(graphics::eProjType::ORTHO)
{
	setDirNormed({ 0, 1, 0 });
}

Camera::Camera(graphics::rProjPersp proj, float nearPlane, float farPlane)
:nearPlane(nearPlane), farPlane(farPlane), pos(0, 0, 0), projPersp(proj), projType(graphics::eProjType::PERSP)
{
	setDirNormed({ 0, 1, 0 });
}

void Camera::setFOV(float rad)
{
	assert(projType == graphics::eProjType::PERSP);

	projPersp.fovRad = rad;
}

void Camera::setNearPlane(float nP)
{
	nearPlane = nP;
}

void Camera::setFarPlane(float fP)
{
	farPlane = fP;
}

void Camera::setPos(const mm::vec3& p)
{
	pos = p;
}

void Camera::setTarget(const mm::vec3& p)
{
	setDirNormed(mm::normalize(p - pos));
}

void Camera::setDirNormed(const mm::vec3& p)
{
	// Important, roll is 0
	const mm::vec3 up(0.0f, 0.0f, 1.0f);

	// TODO FIX THAT MATRIX -> QUAT NOT WORKING
	rot = Matrix44ViewRH(pos, pos + p, up);
}

void Camera::setRot(const mm::quat& q)
{
	rot = q;
}

float Camera::getFOVRad() const
{
	return projPersp.fovRad;
}

float Camera::getNearPlane() const
{
	return nearPlane;
}

float Camera::getFarPlane() const
{
	return farPlane;
}

mm::mat4 Camera::getViewMatrix() const
{
	const mm::vec3 up(0.0f, 0.0f, 1.0f);
	mm::vec3 frontDirNormed = mm::rotate_vector(rot, mm::vec3(0, 1, 0));
	mm::vec3 upDirNormed = mm::rotate_vector(rot, mm::vec3(0, 0, 1));
	return Matrix44ViewRHGL(pos, pos + frontDirNormed, upDirNormed);
}

mm::mat4 Camera::getProjMatrix(float aspectRatio) const
{
	switch (projType)
	{
	case graphics::eProjType::ORTHO:
	{
		return mymath::ortographic(projOrtho.left, projOrtho.right, projOrtho.bottom, projOrtho.top, nearPlane, farPlane);
	}
	case graphics::eProjType::PERSP:
	{
		auto& proj = mymath::perspective(projPersp.fovRad, aspectRatio, nearPlane, farPlane);

		// Remap z from [-1,1] to [0,1]
		return mm::mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, -0.5, 0,
			0, 0, 0.5, 1
			) * proj;
	}
	default:
		assert(0);
		break;
	}
}

mm::vec3 Camera::getDirNormedFront() const
{
	return mm::rotate_vector(rot, mm::vec3(0, 1, 0));
}

mm::vec3 Camera::getDirNormedBack() const
{
	return mm::rotate_vector(rot, mm::vec3(0, -1, 0));
}

mm::vec3 Camera::getDirNormedUp() const
{
	return mm::rotate_vector(rot, mm::vec3(0, 0, 1));
}

mm::vec3 Camera::getDirNormedDown() const
{
	return mm::rotate_vector(rot, mm::vec3(0, 0, -1));
}

mm::vec3 Camera::getDirNormedRight() const
{
	return mm::rotate_vector(rot, mm::vec3(1, 0, 0));
}

mm::vec3 Camera::getDirNormedLeft() const
{
	return mm::rotate_vector(rot, mm::vec3(-1, 0, 0));
}

const mm::vec3& Camera::getPos() const
{
	return pos;
}

const mm::quat& Camera::getRot() const
{
	return rot;
}

const mm::vec3 Camera::getTargetPos() const
{
	return pos + mm::rotate_vector(rot, mm::vec3(0, 1, 0));
}