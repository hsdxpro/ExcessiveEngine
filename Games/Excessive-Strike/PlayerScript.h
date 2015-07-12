#pragma once
#include "Core\Script.h"
#include "mymath\mymath.h"
#include "Core\CameraComponent.h"
#include "Core\RigidBodyComponent.h"
#include "Core\ActorScript.h"

class PlayerScript : public ActorScript
{
public:
	PlayerScript(Actor* actor);

	void Update(float deltaSeconds);

protected:
	// Components
	CameraComponent* camComp;

	RigidBodyComponent* playerCapsule;

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