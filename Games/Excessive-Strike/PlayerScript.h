#pragma once
#include "Core\Script.h"
#include "mymath\mymath.h"
#include "Core\CameraComponent.h"
#include "Core\RigidBodyComponent.h"
#include "Core\EntityScript.h"
#include "Core\GraphicsComponent.h"
#include "Core\Actor.h"

class PlayerScript : public Script
{
public:
	PlayerScript();

	void Update(float deltaSeconds);

protected:
	// Components
	CameraComponent* camComp;

	GraphicsComponent* ak47Graphics;

	Actor* playerCapsule;

	// Moving
	bool bMovingFront;
	bool bMovingBack;
	bool bMovingLeft;
	bool bMovingRight;
	
	float playerMoveSpeed;
	
	// Jump
	bool bCanJump;

	// Cam roting
	float pixelsToRot360;

	mm::vec2 windowCenter;
};