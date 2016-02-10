#pragma once
#include "Core\Script.h"
#include "mymath\mymath.h"
#include "Core\CameraComponent.h"
#include "Core\RigidBodyComponent.h"
#include "Core\ActorScript.h"
#include "Core\MeshComponent.h"
#include "Core\Actor.h"
#include "SoundEngine\IEmitter.h"

class PlayerScript : public Script
{
public:
	PlayerScript();

	void Update(float deltaSeconds);

protected:
	// Components
	CameraComponent* camComp;

	MeshComponent* ak47Graphics;

	Actor* playerCapsule;

	// Moving
	bool bMovingFront;
	bool bMovingBack;
	bool bMovingLeft;
	bool bMovingRight;
	
	float playerMoveSpeed;
	float playerMaxMoveSpeed;

	// Jump
	bool bCanJump;

	// Cam roting
	float pixelsToRot360;

	mm::ivec2 mousePosWhenPress;

	sound::IEmitter* walkSound;
	sound::IEmitter* gunSound;
	sound::IEmitter* shellSound;

	//W, S, A, D down count
	size_t nButtonsDown;

	float shootTimer;
	float rateOfFire;

	bool bSquatting;
	rCollision playerCollisionData;
};