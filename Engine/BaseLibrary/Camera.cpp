// Camera.cpp implementation
#include "Camera.h"

#include <cassert>
#include "mymath/mm_quat_func.h"
#include "GraphicsEngine/RasterZsiros/Core/src/math/vec3.h"
#include "GraphicsEngine/RasterZsiros/Core/src/math/Matrix44.h"

// TODO REMOVE IT OR I KILL MYSELF
//mm::mat4 Matrix44ViewRHGL(const mm::vec3& eye, const mm::vec3& target, const mm::vec3& up)
//{
//	// Negate cuz of OpenGL -z front...
//	mm::vec3 baseFront = mymath::normalize(-(target - eye));		// The "look-at" vector.
//	mm::vec3 baseRight = mymath::normalize(mymath::cross(up, baseFront));	// The "right" vector.
//	mm::vec3 baseUp = mymath::cross(baseFront, baseRight);			// The "up" vector.
//
//	// Create a 4x4 orientation matrix from the right, up, and at vectors
//	// TRANPOSE of ROT
//	mm::mat4 orientation(baseRight.x, baseUp.x, baseFront.x, 0,
//		baseRight.y, baseUp.y, baseFront.y, 0,
//		baseRight.z, baseUp.z, baseFront.z, 0,
//		0, 0, 0, 1);
//
//	// Create a 4x4 translation matrix by negating the eye position.
//	// NEGATE POS
//	mm::mat4 translation(
//		1, 0, 0, 0,
//		0, 1, 0, 0,
//		0, 0, 1, 0,
//		-eye.x, -eye.y, -eye.z, 1);
//
//	return orientation * translation;
//}

mm::mat4 Matrix44ViewRH(const mm::vec3& eye, const mm::vec3& target, const mm::vec3& up)
{
	mm::vec3 zaxis = mm::normalize(eye - target);
	mm::vec3 xaxis = mm::normalize(mm::cross(up, zaxis));
	mm::vec3 yaxis = mm::cross(zaxis, xaxis);

	return mm::mat4(	xaxis.x, yaxis.x, zaxis.x, 0,
						xaxis.y, yaxis.y, zaxis.y, 0,
						xaxis.z, yaxis.z, zaxis.z, 0,
						mm::dot(xaxis, eye), mm::dot(yaxis, eye), mm::dot(zaxis, eye), 1);
}

mm::mat4 Matrix44ViewLH(const mm::vec3& eye, const mm::vec3& target, const mm::vec3& up)
{
	mm::vec3 zaxis = mm::normalize(target - eye);
	mm::vec3 xaxis = mm::normalize(mm::cross(up, zaxis));
	mm::vec3 yaxis = mm::cross(zaxis, xaxis);

	return mm::mat4(	xaxis.x,           yaxis.x,           zaxis.x,          0,
						xaxis.y,           yaxis.y,           zaxis.y,          0,
						xaxis.z,           yaxis.z,           zaxis.z,          0,
						-mm::dot(xaxis, eye), -mm::dot(yaxis, eye), -mm::dot(zaxis, eye), 1.0);
}

Camera::Camera()
:nearPlane(0.2f), farPlane(8000), pos(0, 0, 0), projType(eProjType::PERSP)
{
	SetDirNormed({ 0, 0, 1 });
}

Camera::Camera(rProjOrtho proj, float nearPlane, float farPlane)
:nearPlane(nearPlane), farPlane(farPlane), pos(0, 0, 0), projOrtho(proj), projType(eProjType::ORTHO)
{
	SetDirNormed({ 0, 0, 1 });
}

Camera::Camera(rProjPersp proj, float nearPlane, float farPlane)
:nearPlane(nearPlane), farPlane(farPlane), pos(0, 0, 0), projPersp(proj), projType(eProjType::PERSP)
{
	SetDirNormed({ 0, 0, 1 });
}

void Camera::SetFOV(float rad)
{
	assert(projType == eProjType::PERSP);

	projPersp.fovRad = rad;
}

void Camera::SetNearPlane(float nP)
{
	nearPlane = nP;
}

void Camera::SetFarPlane(float fP)
{
	farPlane = fP;
}

void Camera::SetPos(const mm::vec3& p)
{
	pos = p;
}

void Camera::SetTarget(const mm::vec3& p)
{
	SetDirNormed(mm::normalize(p - pos));
}

void Camera::SetDirNormed(const mm::vec3& p)
{
	// Important, roll is 0
	const mm::vec3 up(0.0f, 1.0f, 0.0f);

	auto mat = mm::inverse(Matrix44ViewLH(pos, pos + p, up));
	// TODO FIX THAT MATRIX -> QUAT NOT WORKING
	rot = (mm::quat)mat;

	//mm::vec3 asd = mm::rotate_vector(rot, mm::vec3(1, 0, 0));
	//asd = asd;
}

void Camera::SetRot(const mm::quat& q)
{
	rot = q;
}

float Camera::GetFOVRad() const
{
	return projPersp.fovRad;
}

float Camera::GetNearPlane() const
{
	return nearPlane;
}

float Camera::GetFarPlane() const
{
	return farPlane;
}

mm::mat4 Camera::GetViewMatrix() const
{
	const Vec3 up(0.0f, 0.0f, 1.0f);

	mm::vec3 frontDirNormed = mm::rotate_vector(rot, mm::vec3(0, 0, 1));
	mm::vec3 upDirNormed = mm::vec3(0, 1, 0);// mm::rotate_vector(rot, mm::vec3(0, 0, 1));

	mm::mat4 mat =  Matrix44ViewLH(pos, pos + frontDirNormed, upDirNormed);

	mm::vec4 vec = mat * mm::vec4(0.0, 0.0, 1.0, 1.0);
	vec = vec;

	return mat;
}

mm::mat4 Camera::GetProjMatrix(float aspectRatio) const
{
	switch (projType)
	{
	case eProjType::ORTHO:
	{
		return mymath::ortographic(projOrtho.left, projOrtho.right, projOrtho.bottom, projOrtho.top, nearPlane, farPlane);
	}
	case eProjType::PERSP:
	{
		Matrix44 ad = Matrix44ProjPerspectiveLH(nearPlane, farPlane, projPersp.fovRad, aspectRatio);

		memcpy((float*)&proj[0], (float*)&ad[0], sizeof(float) * 16);
		mm::mat4 res = proj;

		return res;

		//auto& proj = mymath::perspective(projPersp.fovRad, aspectRatio, nearPlane, farPlane);

		// Remap z from [-1,1] to [0,1]
		//return mm::mat4(
		//	1, 0, 0, 0,
		//	0, 1, 0, 0,
		//	0, 0, -0.5, 0,
		//	0, 0, 0.5, 1
		//	) * proj;
	}
	default:
		assert(0);
		break;
	}

	return mm::mat4();
}

mm::vec3 Camera::GetFrontDir() const
{
	return mm::rotate_vector(rot, mm::vec3(0, 0, 1));
}

mm::vec3 Camera::GetBackDir() const
{
	return mm::rotate_vector(rot, mm::vec3(0, 0, -1));
}

mm::vec3 Camera::GetUpDir() const
{
	return mm::rotate_vector(rot, mm::vec3(0, 1, 0));
}

mm::vec3 Camera::GetDownDir() const
{
	return mm::rotate_vector(rot, mm::vec3(0, -1, 0));
}

mm::vec3 Camera::GetRightDir() const
{
	return mm::rotate_vector(rot, mm::vec3(1, 0, 0));
}

mm::vec3 Camera::GetLeftDir() const
{
	return mm::rotate_vector(rot, mm::vec3(-1, 0, 0));
}

const mm::vec3& Camera::GetPos() const
{
	return pos;
}

const mm::quat& Camera::GetRot() const
{
	return rot;
}

const mm::vec3 Camera::GetTargetPos() const
{
	return pos + mm::rotate_vector(rot, mm::vec3(0, 0, 1));
}