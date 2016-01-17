#include "PlayerScript.h"
#include "PlatformLibrary\Sys.h"
#include "ExcessiveStrikeCommon.h"
#include <windows.h>

PlayerScript::PlayerScript()
{
	bSquatting = false;

	rateOfFire = 0.1f; // ak 0.1
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
	windowCenter = Graphics.GetTargetWindow()->GetCenterPos();

	// Weapon model
	//auto& ak47ModelPath = "ak47/ak.obj";
	auto& ak47ModelPath = "ak47/ak47.dae";

	// Camera
	camComp = World.SpawnComp_Camera();
	Graphics.SetCam(camComp);
	
	// Player components
	playerCapsule = World.SpawnActor_RigidBodyCapsule(2, 0.2f, 70);
	playerCapsule->SetCollisionGroup(eES_CollisionGroup::PLAYER);
	playerCapsule->SetOnCollision([&](const rCollision& col)
	{
		playerCollisionData = col;
	});

	playerCapsule->SetOnCollisionEnter([&](const rCollision& col)
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

	playerCapsule->SetAngularFactor(0);
	playerCapsule->SetKinematic(true);

	// Attach camera to player physics
	camComp->Move({ 0, 0, 1 });
	playerCapsule->Attach(camComp);
	playerCapsule->SetPos({ 0, 0, 2 });
	
	ak47Graphics = World.SpawnComp_MeshFromFile(ak47ModelPath);

	ak47Graphics->SetScaleLocal({ 1.f / 3, 1.f / 3, 1.f / 3 });
	ak47Graphics->SetPos(camComp->GetPos() + mm::vec3(0.7f, 1.5f, -0.6f));
	ak47Graphics->Rot(mm::quat(-90.f / 180.f * 3.1415f, { 0, 0, 1 }));

	// Old trans
	//ak47Graphics->SetScaleLocal({ 1.f / 1800, 1.f / 1800, 1.f / 1800 });
	//ak47Graphics->SetRot(mm::quat(-0.05f, { 1, 0, 0 }) * mm::quat(3.1415f - 0.08f, { 0, 0, 1 }) * mm::quat(3.1415f / 2, { 1, 0, 0 })); // Need quaternion rework, multiply swapped, mm::quat(angle) rots with -angle, why?
	//ak47Graphics->SetPos(camComp->GetPos() + mm::vec3(0.7f, 1.05f - 0.15f, -0.6f));
	//ak47Graphics->Rot(mm::quat(7.f / 180.f * 3.1415f, { 0, 0, 1 }) * mm::quat(-3.f / 180.f * 3.1415f, { 1, 0, 0 }));
	camComp->Attach(ak47Graphics); // Attach weapon to player physics
	
	// Ha ez a sor bevan tolva akkor debug - ban lezuhanunk, olyan mintha scale = 0 lenne
	playerCapsule->ScaleLocal({ 1.f / 3.5f, 1.f / 3.5f, 1.f / 3.5f });

	walkSound = Sound.CreateSoundMono("walk_sound.ogg", 1, true);
	gunSound = Sound.CreateSoundMono("GUN_FIRE-stereo.ogg", 0.5);
	shellSound = Sound.CreateSoundMono("shell_fall.ogg", 0.5);

	playerCapsule->SetPos(mm::vec3(0, 0, 5));
}

void PlayerScript::Update(float deltaSeconds)
{
	// W,S,A,D Moving
	nButtonsDown = 0;
	mm::vec3 move(0, 0, 0);
	if (Input.IsKeyDown(eKey::W))
	{
		move += camComp->GetFrontDirNormed();
		nButtonsDown++;
	}
	if (Input.IsKeyDown(eKey::S))
	{
		move += camComp->GetBackDirNormed();
		nButtonsDown++;
	}
	if (Input.IsKeyDown(eKey::A))
	{
		move += camComp->GetLeftDirNormed();
		nButtonsDown++;
	}
	if (Input.IsKeyDown(eKey::D))
	{
		move += camComp->GetRightDirNormed();
		nButtonsDown++;
	}

	// Decline newVel components facing to player self contact normals
	for (auto& contact : playerCollisionData.contacts)
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

	if (nButtonsDown == 1 && Input.IsKeyPressed(eKey::W) | Input.IsKeyPressed(eKey::S) | Input.IsKeyPressed(eKey::A) | Input.IsKeyPressed(eKey::D))
		walkSound->Start();

	if (nButtonsDown == 0 && Input.IsKeyReleased(eKey::W) | Input.IsKeyReleased(eKey::S) | Input.IsKeyReleased(eKey::A) | Input.IsKeyReleased(eKey::D))
		walkSound->Stop();

	// Jump
	if (bCanJump && Input.IsKeyPressed(eKey::SPACE))
	{
		bCanJump = false;
		playerCapsule->AddForce({ 0, 0, 15000 });
		//walkSound->Stop();
	}

	// Squat
	if (!bSquatting && Input.IsKeyPressed(eKey::LCTRL))
	{
		camComp->MoveRel(mm::vec3(0, 0, -0.8f));
		bSquatting = true;
		playerMoveSpeed /= 2;
	}
	else if (bSquatting && Input.IsKeyReleased(eKey::LCTRL))
	{
		camComp->MoveRel(mm::vec3(0, 0, 0.8f));
		bSquatting = false;
		playerMoveSpeed = playerMaxMoveSpeed;
	}

	if (shootTimer > 0)
		shootTimer -= deltaSeconds;

	if (Input.IsMouseLeftDown() && shootTimer <= 0)
	{
		shootTimer =+ rateOfFire;

		gunSound->Start();

		// Falling bullet shell, and it's sound
		Actor* bulletShell = World.SpawnActor_RigidBodyCapsule(0.09f, 0.04f, 0.02f);
		bulletShell->SetTrigger(true);
		bulletShell->SetCollisionGroup(eES_CollisionGroup::SHELL);
		bulletShell->SetPos(ak47Graphics->GetPos());
		bulletShell->SetOnCollisionEnter([=](const rCollision& info)
		{
			shellSound->Start();
			World.Destroy(bulletShell);
		});
		
		
		rPhysicsTraceResult result;
		rPhysicsTraceParams params;
		params.AddIgnoreCollisionLayer(eES_CollisionGroup::SHELL);

		//params. Trace pls ignoráld már a shelleket
		if (Physics.TraceClosestPoint(camComp->GetPos(), camComp->GetPos() + camComp->GetFrontDirNormed() * 999999, result, params))
		{
			//MeshComponent* boxComp = World.SpawnComp_MeshFromFile("box.DAE");
			MeshComponent* boxComp = World.SpawnComp_MeshFromFile("sziv.DAE");
			boxComp->SetScaleLocal({ 1.f / 2, 1.f / 2, 1.f / 2});
			boxComp->SetPos(result.pos);//cuki <3 <3 <3 I <3 U Rici
			boxComp->SetRot(camComp->GetRot());
		}

		//Actor* bullet = Core.SpawnActor_RigidBodyFromFile("box.DAE", 100);
		//bullet->Attach(Core.SpawnComp_MeshFromFile("box.DAE"));
		//bullet->SetScaleLocal({ 1.f / 3, 1.f / 3, 1.f / 3 });
		//
		//bullet->SetCollisionGroup(eES_CollisionGroup::BULLET);
		//
		//mm::vec3 bulletDirNormed = camComp->GetFrontDirNormed();
		//bullet->SetPos(ak47Graphics->GetPos());
		//bullet->SetVelocity(bulletDirNormed * 7);
		//bullet->Scale(bulletDirNormed * 3);
	}


	if (Input.IsMouseRightPressed())
	{
		mousePosWhenPress = Sys::GetCursorPos();
		Sys::SetCursorVisible(false);
		Sys::SetCursorPos(mm::uvec2((u32)mousePosWhenPress.x, (u32)mousePosWhenPress.y));
	}

	if (Input.IsMouseRightReleased())
		Sys::SetCursorVisible(true);

	if (Input.IsMouseRightDown())
	{
		// Roting camera
		mm::uvec2 mouseDelta;
		if (Input.IsMouseMove(mouseDelta))
		{
			static float angleZ = 0;
			static float angleX = 0;

			// Input read up finished, now we can recenter cursor for our fps game
			auto mousePos = Sys::GetCursorPos();

			float mouseDx = mousePos.x - mousePosWhenPress.x;
			float mouseDy = mousePos.y - mousePosWhenPress.y;

			angleZ += -(float)mouseDx / pixelsToRot360 * 6.28;
			angleX += -(float)mouseDy / pixelsToRot360 * 6.28;

			// Clamp angleX
			float angleSign = angleX >= 0 ? 1 : -1;
			if (angleX * angleSign >= 3.14159265 / 2 * 0.95)
				angleX = 3.14159265 / 2 * 0.95 * angleSign;

			mm::quat rotAroundZ(angleZ, { 0, 0, 1 });
			mm::quat rotAroundX(angleX, { 1, 0, 0 });

			camComp->SetRot(rotAroundZ * rotAroundX);
		}

		Sys::SetCursorPos(mm::uvec2((u32)mousePosWhenPress.x, (u32)mousePosWhenPress.y));
	}
}