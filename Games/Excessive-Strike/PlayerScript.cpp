#include "PlayerScript.h"
#include "PlatformLibrary\Sys.h"
#include "Core\Core.h"
#include "Core\Input.h"
#include "ExcessiveStrikeCommon.h"

PlayerScript::PlayerScript()
{
	bMovingFront = false;
	bMovingBack = false;
	bMovingLeft = false;
	bMovingRight = false;
	bCanJump = true;
	playerMoveSpeed = 10;
	pixelsToRot360 = 1000;
	windowCenter = gCore->GetTargetWindow()->GetCenterPos();

	// Weapon model
	auto& ak47ModelPath = L"Assets/ak47/ak.obj";
	
	// Camera
	camComp = gCore->SpawnComp_Camera();
	gCore->SetCam(camComp);
	
	//// Player components
	playerCapsule = gCore->SpawnComp_RigidBodyCapsule(2, 0.2f, 20);
	playerCapsule->SetAngularFactor(0);
	
	// Attach camera to player physics
	camComp->Move({ 0, 0, 1 });
	playerCapsule->Attach(camComp);
	playerCapsule->SetPos({ 0, 0, 2 });
	
	ak47Graphics = gCore->SpawnComp_MeshFromFile(ak47ModelPath);
	ak47Graphics->SetScaleLocal({ 1.f / 1500, 1.f / 1500, 1.f / 1500 });
	ak47Graphics->SetRot(mm::quat(-0.05f, { 1, 0, 0 }) * mm::quat(3.1415f - 0.08f, { 0, 0, 1 }) * mm::quat(3.1415f / 2, { 1, 0, 0 })); // Need quaternion rework, multiply swapped, mm::quat(angle) rots with -angle, why?
	ak47Graphics->SetPos(camComp->GetPos() + mm::vec3(0.7f, 1.05f, -0.6f));
	ak47Graphics->Rot(mm::quat(7.f / 180.f * 3.1415f, { 0, 0, 1 }) * mm::quat(-3.f / 180.f * 3.1415f, { 1, 0, 0 }));
	camComp->Attach(ak47Graphics); // Attach weapon to player physics
	
	// Mouse recenter
	mm::vec2 windowCenter = gCore->GetTargetWindow()->GetCenterPos();
	Sys::SetCursorPos(mm::uvec2((u32)windowCenter.x, (u32)windowCenter.y));

	gCore->PlaySoundMono(L"Assets/PurgatorysMansion-mono.ogg", 1, true);
}

void PlayerScript::Update(float deltaSeconds)
{
	// W,S,A,D Moving
	mm::vec3 dMove(0, 0, 0);
	if(gInput.IsKeyDown(eKey::W))
		dMove += camComp->GetDirFrontNormed() * deltaSeconds * playerMoveSpeed;
	if(gInput.IsKeyDown(eKey::S))
		dMove += camComp->GetDirBackNormed() * deltaSeconds * playerMoveSpeed;
	if(gInput.IsKeyDown(eKey::A))
		dMove += camComp->GetDirLeftNormed() * deltaSeconds * playerMoveSpeed;
	if(gInput.IsKeyDown(eKey::D))
		dMove += camComp->GetDirRightNormed() * deltaSeconds * playerMoveSpeed;
	dMove.z = 0;
	playerCapsule->Move(dMove);

	// Jump
	if(gInput.IsKeyPressed(eKey::SPACE))
	{
		bCanJump = false;
		playerCapsule->AddForce({ 0, 0, 5100 });
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

	if (gInput.IsMouseLeftPressed())
	{
		gCore->PlaySoundMono(L"Assets/GUN_FIRE-stereo.ogg", 0.5);

		// Falling bullet shell, and it's sound
		Actor* bulletShell = gCore->SpawnActor_RigidBodyCapsule(0.09, 0.04, 0.02);
		bulletShell->SetTrigger(true);
		bulletShell->SetCollisionGroup(eES_CollisionGroup::SHELL);
		bulletShell->SetPos(ak47Graphics->GetPos());
		bulletShell->SetOnCollisionEnter([=](const rCollision& info)
		{
			gCore->PlaySoundMono(L"Assets/shell_fall.ogg", 0.5);
			gCore->DestroyActor(bulletShell);
		});

		// Shooted bullet
		Actor* bullet = gCore->SpawnActor_RigidBodyFromFile(L"Assets/box.DAE", 1);
		bullet->Attach(gCore->SpawnComp_MeshFromFile(L"Assets/box.DAE"));
		bullet->SetScaleLocal({ 1.f / 100, 1.f / 100, 1.f / 100 });
		bullet->SetGravityScale(0);

		mm::vec3 bulletDir = ak47Graphics->GetDirUpNormed();
		bullet->SetPos(ak47Graphics->GetPos() + bulletDir * 2);

		mm::vec3 bulletStartPos = bullet->GetPos();
		const float bulletSpeed = 100;
		const float distAfterDisappear = 200;
		bullet->SetOnUpdate([=](float deltaSeconds)
		{
			bullet->Move(deltaSeconds * bulletDir * bulletSpeed);
		
			if (mm::length(bulletStartPos - bullet->GetPos()) >= distAfterDisappear)
				gCore->DestroyActor(bullet);
		});
	}

	// Mouse recenter
	Sys::SetCursorPos(mm::uvec2((u32)windowCenter.x, (u32)windowCenter.y));
}