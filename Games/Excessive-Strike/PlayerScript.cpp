#include "PlayerScript.h"
#include "PlatformLibrary\Sys.h"
#include "ExcessiveStrikeCommon.h"

PlayerScript::PlayerScript()
{
	bSquatting = false;

	rateOfFire = 0.1f; // ak 0.1 sec per bullett
	shootTimer = rateOfFire;

	nButtonsDown = 0;

	bMovingFront = false;
	bMovingBack = false;
	bMovingLeft = false;
	bMovingRight = false;
	bCanJump = true;
	playerMaxMoveSpeed = 2.6f;
	playerMoveSpeed = playerMaxMoveSpeed;
	pixelsToRot360 = 1000;

	// Weapon model
	auto& ak47ModelPath = "ak47/ak47.dae";
	
	// Physics capsule for player
	playerCapsule = World.AddActor_RigidBodyCapsule(2, 0.2f, 70);
	playerCapsule->SetAngularFactor(0);
	playerCapsule->SetKinematic(true);
	playerCapsule->SetCollisionGroup(eES_CollisionGroup::PLAYER);

	playerCapsule->SetOnCollisionEnter([&](const Collision& col)
	{
		if (col.actorB && col.actorB->GetName() == "ground")
		{
			bool bStayOnGroundThing = false;

			for (auto& contact : col.contacts)
				if (contact.normalB.z > 0.707)
					bStayOnGroundThing = true;

			if (bStayOnGroundThing)
			{
				if (nButtonsDown > 0 && !walkSound->IsEmitting())
					walkSound->Start();

				bCanJump = true;
			}
		}
	});

	// Camera
	camComp = World.AddComponent_Camera();
	camComp->Move(mm::vec3( 0,0,1));
	
	// Attach to rigid body capsule
	playerCapsule->GetRootComponent(0)->Attach(camComp);

	ak47Graphics = camComp->AddComponent_Mesh(ak47ModelPath);
	ak47Graphics->SetScale(1.0 / 3);
	ak47Graphics->Move(mm::vec3(0.7f, 1.5f, -0.6f));
	ak47Graphics->RotZ(-90);
	ak47Graphics->RotY(-90);
	
	playerCapsule->Scale(1.0 / 3);

	walkSound = Sound.CreateMonoSound("walk_sound.ogg", 1, true);
	gunSound = Sound.CreateMonoSound("GUN_FIRE-stereo.ogg", 0.5);
	shellSound = Sound.CreateMonoSound("shell_fall.ogg", 0.5);

	playerCapsule->SetPos(mm::vec3(0, 0, 5));

	Core.SetCam(camComp);
}

void PlayerScript::Update(float deltaSeconds)
{
	// W,S,A,D Moving
	nButtonsDown = 0;
	mm::vec3 move(0, 0, 0);

	if (Input.IsKeyDown(W))
	{
		move += camComp->GetFrontDir();
		nButtonsDown++;
	}
	if (Input.IsKeyDown(S))
	{
		move += camComp->GetBackDir();
		nButtonsDown++;
	}
	if (Input.IsKeyDown(A))
	{
		move += camComp->GetLeftDir();
		nButtonsDown++;
	}
	if (Input.IsKeyDown(D))
	{
		move += camComp->GetRightDir();
		nButtonsDown++;
	}

	// Decline newVel components facing to player self contact normals
	for (ContactPoint& contact : playerCapsule->GetContactPoints())
	{
		mm::vec3 playerToOtherContactNormal = contact.normalA;

		// move = projectVecToPlane(move, plane(playerToOtherContactNormal, contact.posA));
		// plane(playerToOtherContactNormal, contact.posA)
	}

	move.z = 0;
	if (move.x != 0 || move.y != 0)
		move = mm::normalize(move);

	move *= playerMoveSpeed;

	playerCapsule->SetVelocity(mm::vec3(move.x, move.y, playerCapsule->GetVelocity().z));

	if (nButtonsDown == 1 && Input.IsKeyPressed(W) | Input.IsKeyPressed(S) | Input.IsKeyPressed(A) | Input.IsKeyPressed(D))
		walkSound->Start();

	if (nButtonsDown == 0 && Input.IsKeyReleased(W) | Input.IsKeyReleased(S) | Input.IsKeyReleased(A) | Input.IsKeyReleased(D))
		walkSound->Stop();

	// Jump
	if (bCanJump && Input.IsKeyPressed(SPACE))
	{
		bCanJump = false;
		playerCapsule->AddForce({ 0, 0, 15000 });
		//walkSound->Stop();
	}

	// Squat
	if (!bSquatting && Input.IsKeyPressed(LCTRL))
	{
		camComp->MoveRel(mm::vec3(0, 0, -0.8f));
		bSquatting = true;
		playerMoveSpeed /= 2;
	}
	else if (bSquatting && Input.IsKeyReleased(LCTRL))
	{
		camComp->MoveRel(mm::vec3(0, 0, 0.8f));
		bSquatting = false;
		playerMoveSpeed = playerMaxMoveSpeed;
	}

	if (shootTimer > 0)
		shootTimer -= deltaSeconds;

	if (Input.IsLeftMouseBtnDown() && shootTimer <= 0)
	{
		shootTimer =+ rateOfFire;

		gunSound->Start();

		// Falling bullet shell, and it's sound
		//Actor* bulletShell = World.AddActor_RigidBodyCapsule(0.09f, 0.04f, 0.02f);
		//bulletShell->SetTrigger(true);
		//bulletShell->SetCollisionGroup(eES_CollisionGroup::SHELL);
		//bulletShell->SetPos(ak47Graphics->GetPos());
		//bulletShell->SetOnCollisionEnter([=](const Collision& info)
		//{
		//	shellSound->Start();
		//	World.Remove(bulletShell);
		//});
		
		
		PhysicsTraceResult result;
		PhysicsTraceParams params;
		params.AddIgnoreCollisionLayer(eES_CollisionGroup::SHELL);

		//params. Trace pls ignoráld már a shelleket
		//if (Physics.TraceClosestPoint(camComp->GetPos(), camComp->GetPos() + camComp->GetFrontDir() * 999999, result, params))
		//{
		//	Actor* boxComp = World.AddMesh("box.DAE");
		//	//Actor* boxComp = World.AddMesh("sziv.DAE");
		//	boxComp->SetPos(result.pos);//cuki <3 <3 <3 I <3 U Rici
		//	//boxComp->SetRot(camComp->GetRot());
		//}

		// TODO Ha kell egy sorból tudjak rigidBody Mesh és Sound
		Actor* bullet = World.AddActor("box.DAE", 1);
		bullet->SetScale(1.0 / 3);

		bullet->SetCollisionGroup(eES_CollisionGroup::BULLET);
		
		mm::vec3 bulletDirNormed = camComp->GetFrontDir();
		bullet->SetPos(ak47Graphics->GetPos() + bulletDirNormed);
		bullet->SetVelocity(bulletDirNormed * 2);
		//bullet->Scale(bulletDirNormed * 3);
	}

	if (Input.IsRightMouseBtnPressed())
	{
		mousePosWhenPress = Sys::GetCursorPos();
		Sys::SetCursorVisible(false);
	}

	if (Input.IsRightMouseBtnReleased())
	{
		Sys::SetCursorPos(mm::ivec2(mousePosWhenPress.x, mousePosWhenPress.y));
		Sys::SetCursorVisible(true);
	}

	if (Input.IsRightMouseBtnDown())
	{
		// Roting camera
		mm::ivec2 mouseDelta;
		if (Input.IsMouseMove(mouseDelta))
		{
			static float angleZ = 0;
			static float angleX = 0;

			// Input read up finished, now we can recenter cursor for our fps game
			//auto mousePos = Sys::GetCursorPos();

			float mouseDx = mouseDelta.x;
			float mouseDy = mouseDelta.y;

			angleZ += -(float)mouseDx / pixelsToRot360 * 6.28;
			angleX += -(float)mouseDy / pixelsToRot360 * 6.28;

			// Clamp angleX
			float angleSign = angleX >= 0 ? 1 : -1;
			if (angleX * angleSign >= 3.14159265 / 2 * 0.95)
				angleX = 3.14159265 / 2 * 0.95 * angleSign;

			mm::quat rotAroundZ(angleZ, { 0, 0, 1 });
			mm::quat rotAroundX(angleX, { 1, 0, 0 });

			camComp->SetRot(rotAroundZ * rotAroundX);
			Sys::SetCursorPos(mm::ivec2(mousePosWhenPress.x, mousePosWhenPress.y));
		}
	}
}