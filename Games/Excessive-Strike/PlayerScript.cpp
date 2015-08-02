#include "PlayerScript.h"
#include "PlatformLibrary\Sys.h"
#include "Core\Core.h"
#include "Core\Input.h"
#include "ExcessiveStrikeCommon.h"

rCollision playerCollisionData;

PlayerScript::PlayerScript()
{
	bSquatting = false;

	rateOfFire = 0.1f;
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
	windowCenter = gCore->GetTargetWindow()->GetCenterPos();

	// Weapon model
	auto& ak47ModelPath = "Assets/ak47/ak.obj";
	//auto& ak47ModelPath = "Assets/ak47/ak47.obj";

	// Camera
	camComp = gCore->SpawnComp_Camera();
	gCore->SetCam(camComp);
	
	// Player components
	playerCapsule = gCore->SpawnActor_RigidBodyCapsule(2, 0.2f, 70);
	playerCapsule->SetCollisionGroup(eES_CollisionGroup::PLAYER);
	playerCapsule->SetOnCollision([&](const rCollision& col)
	{
		playerCollisionData = col;
	});

	playerCapsule->SetOnCollisionEnter([&](const rCollision& col)
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
	});

	playerCapsule->SetAngularFactor(0);
	playerCapsule->SetKinematic(true);

	// Attach camera to player physics
	camComp->Move({ 0, 0, 1 });
	playerCapsule->Attach(camComp);
	playerCapsule->SetPos({ 0, 0, 2 });
	
	ak47Graphics = gCore->SpawnComp_MeshFromFile(ak47ModelPath);

	//ak47Graphics->SetScaleLocal({ 1.f / 100, 1.f / 100, 1.f / 100 });
	//ak47Graphics->SetPos(camComp->GetPos() + mm::vec3(0.7f, 1.5f, -0.6f));
	//ak47Graphics->Rot(mm::quat(-90.f / 180.f * 3.1415f, { 0, 0, 1 }));
	//ak47Graphics->Rot(mm::quat(-90.f / 180.f * 3.1415f, { 0, 1, 0 }));

	// Old trans
	ak47Graphics->SetScaleLocal({ 1.f / 1800, 1.f / 1800, 1.f / 1800 });
	ak47Graphics->SetRot(mm::quat(-0.05f, { 1, 0, 0 }) * mm::quat(3.1415f - 0.08f, { 0, 0, 1 }) * mm::quat(3.1415f / 2, { 1, 0, 0 })); // Need quaternion rework, multiply swapped, mm::quat(angle) rots with -angle, why?
	ak47Graphics->SetPos(camComp->GetPos() + mm::vec3(0.7f, 1.05f - 0.15f, -0.6f));
	ak47Graphics->Rot(mm::quat(7.f / 180.f * 3.1415f, { 0, 0, 1 }) * mm::quat(-3.f / 180.f * 3.1415f, { 1, 0, 0 }));
	camComp->Attach(ak47Graphics); // Attach weapon to player physics
	
	// Mouse recenter
	mm::vec2 windowCenter = gCore->GetTargetWindow()->GetCenterPos();
	Sys::SetCursorPos(mm::uvec2((u32)windowCenter.x, (u32)windowCenter.y));

	//gCore->PlaySoundMono(L"Assets/PurgatorysMansion-mono.ogg", 1, true);

	// Ha ez a sor bevan tolva akkor debug - ban lezuhanunk, olyan mintha scale = 0 lenne
	playerCapsule->ScaleLocal({ 1.f / 3.5f, 1.f / 3.5f, 1.f / 3.5f });

	walkSound = gCore->CreateSoundMono("Assets/walk_sound.ogg", 1, true);
	gunSound = gCore->CreateSoundMono("Assets/GUN_FIRE-stereo.ogg", 0.5);
	shellSound = gCore->CreateSoundMono("Assets/shell_fall.ogg", 0.5);

	playerCapsule->SetPos(mm::vec3(0, 0, 3));
}

void PlayerScript::Update(float deltaSeconds)
{
	// W,S,A,D Moving
	nButtonsDown = 0;
	mm::vec3 move(0, 0, 0);
	if (gInput.IsKeyDown(eKey::W))
	{
		move += camComp->GetDirFrontNormed();
		nButtonsDown++;
	}
	if (gInput.IsKeyDown(eKey::S))
	{
		move += camComp->GetDirBackNormed();
		nButtonsDown++;
	}
	if (gInput.IsKeyDown(eKey::A))
	{
		move += camComp->GetDirLeftNormed();
		nButtonsDown++;
	}
	if (gInput.IsKeyDown(eKey::D))
	{
		move += camComp->GetDirRightNormed();
		nButtonsDown++;
	}

	// Decline newVel components facing to player self contact normals
	for (auto& contact : playerCollisionData.contacts)
	{
		mm::vec3 playerToOtherContactNormal = contact.normalA;

		// move = projectVecToPlane(move, plane(playerToOtherContactNormal, contact.posA));
		// plane(playerToOtherContactNormal, contact.posA)
	}

	if (move.x != 0 || move.y != 0 || move.z != 0)
		move = mm::normalize(move);

	move *= playerMoveSpeed;

	playerCapsule->SetVelocity(mm::vec3(move.x, move.y, playerCapsule->GetVelocity().z));


	if (nButtonsDown == 1 && gInput.IsKeyPressed(eKey::W) | gInput.IsKeyPressed(eKey::S) | gInput.IsKeyPressed(eKey::A) | gInput.IsKeyPressed(eKey::D))
		walkSound->Start();

	if (nButtonsDown == 0 && gInput.IsKeyReleased(eKey::W) | gInput.IsKeyReleased(eKey::S) | gInput.IsKeyReleased(eKey::A) | gInput.IsKeyReleased(eKey::D))
		walkSound->Stop();

	// Jump
	if (bCanJump && gInput.IsKeyPressed(eKey::SPACE))
	{
		bCanJump = false;
		playerCapsule->AddForce({ 0, 0, 15000 });
		walkSound->Stop();
	}

	// Squat
	if (!bSquatting && gInput.IsKeyPressed(eKey::LCONTROL))
	{
		camComp->MoveRel(mm::vec3(0, 0, -0.8f));
		bSquatting = true;
		playerMoveSpeed /= 2;
	}
	else if (bSquatting && gInput.IsKeyReleased(eKey::LCONTROL))
	{
		camComp->MoveRel(mm::vec3(0, 0, 0.8f));
		bSquatting = false;
		playerMoveSpeed = playerMaxMoveSpeed;
	}

	// Roting camera
	mm::uvec2 mouseDelta;
	if(gInput.IsMouseMove(mouseDelta))
	{
		static float angleZ = 0;
		static float angleX = 0;

		// Input read up finished, now we can recenter cursor for our fps game
		auto mousePos = Sys::GetCursorPos();

		float mouseDx = mousePos.x - windowCenter.x;
		float mouseDy = mousePos.y - windowCenter.y;

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

	if (shootTimer > 0)
		shootTimer -= deltaSeconds;

	if (gInput.IsMouseLeftDown() && shootTimer <= 0)
	{
		shootTimer =+ rateOfFire;

		gunSound->Start();

		// Falling bullet shell, and it's sound
		Actor* bulletShell = gCore->SpawnActor_RigidBodyCapsule(0.09, 0.04, 0.02);
		bulletShell->SetTrigger(true);
		bulletShell->SetCollisionGroup(eES_CollisionGroup::SHELL);
		bulletShell->SetPos(ak47Graphics->GetPos());
		bulletShell->SetOnCollisionEnter([=](const rCollision& info)
		{
			shellSound->Start();
			gCore->DestroyActor(bulletShell);
		});
		
		//// Shooted bullet
		Actor* bullet = gCore->SpawnActor_RigidBodyFromFile("Assets/box.DAE", 100); // mass = 1
		bullet->Attach(gCore->SpawnComp_MeshFromFile("Assets/box.DAE"));
		bullet->SetScaleLocal({ 1.f / 100, 1.f / 100, 1.f / 100 });
		bullet->SetCollisionGroup(eES_CollisionGroup::BULLET);

		mm::vec3 bulletDir = camComp->GetDirFrontNormed();
		bullet->SetPos(ak47Graphics->GetPos());
		bullet->SetVelocity(bulletDir * 7);

		//mm::vec3 bulletStartPos = bullet->GetPos();
		//const float bulletSpeed = 100;
		//const float distAfterDisappear = 200;
		//bullet->SetOnUpdate([=](float deltaSeconds)
		//{
		//	bullet->Move(deltaSeconds * bulletDir * bulletSpeed);
		//
		//	if (mm::length(bulletStartPos - bullet->GetPos()) >= distAfterDisappear)
		//		gCore->DestroyActor(bullet);
		//});
	}

	// Mouse recenter
	Sys::SetCursorPos(mm::uvec2((u32)windowCenter.x, (u32)windowCenter.y));
}