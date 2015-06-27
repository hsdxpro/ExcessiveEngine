#include "..\Common\src\Factory.h"
#include "..\Common\src\IWindow.h"
#include "..\Common\src\Sys.h"
#include "..\Common\src\ITimer.h"
#include "..\Common\src\VisualCpuProfiler.h"

#include "..\Core\src\EngineCore.h"

// Spirit of the engine
EngineCore gEngineCore;

// Window for our game
IWindow* gWindow;

ComponentCamera* gMainCam;

int main()
{
	// Full screen popup window for our game
	rWindow d;
		//d.style = eWindowStyle::TITLE__RESIZE__CLOSE; // Windowded props
		//d.clientW = 800; // Windowded props
		//d.clientH = 600; // Windowded props
		d.clientW = Sys::GetScreenSize().x; //FULL SCREEN props
		d.clientH = Sys::GetScreenSize().y; //FULL SCREEN props
		d.style = eWindowStyle::FULLSCREEN; //FULL SCREEN props
	gWindow = Factory::CreateWindow(d);

	// Hide hardware cursor for our game on window
	gWindow->HideCursor();

	// Init Engine core
	rGraphicsEngineRaster graphicsDesc;
		graphicsDesc.gapiType = eGapiType::OPENGL_4_5;
		graphicsDesc.targetWindow = gWindow;
	gEngineCore.InitGraphicsEngineRaster(graphicsDesc);
	gEngineCore.InitPhysicsEngineBullet();
	gEngineCore.InitSoundEngine();

	ITimer* timer = Factory::CreateTimer();
	timer->Start();

	// Create camera
	//auto cam = gEngineCore.GetGraphicsEngine()->CreateCam();
	//	cam->SetPos(mm::vec3(0, -3, 10));
	//gEngineCore.GetDefaultGraphicsScene()->SetCamera(cam);

	// Add some actors
	auto& groundModelPath = Sys::GetWorkDir() + L"../Assets/demo_ground.dae";
	auto& skyModelPath = Sys::GetWorkDir() + L"../Assets/skybox.dae";
	auto& ak47ModelPath = Sys::GetWorkDir() + L"../Assets/ak47/ak.obj";
	auto& boxModelPath = Sys::GetWorkDir() + L"../Assets/box.dae";

	// Terrain & Sky
	gEngineCore.AddCompRigidBodyFromFile(groundModelPath, 0);
	gEngineCore.AddCompGraphicsFromFile(groundModelPath);
	gEngineCore.AddCompGraphicsFromFile(skyModelPath)->SetScaleLocal({ 1000, 1000, 1000 });

	// Camera
	gMainCam = gEngineCore.AddCompCamera();
	gEngineCore.SetCam(gMainCam);

	//// Player components
	auto playerCapsule = gEngineCore.AddCompRigidBodyCapsule(2, 0.2, 20);
		playerCapsule->SetAngularFactor(0);
		//playerCapsule->SetKinematic();
	
	// Attach camera to player physics
	gMainCam->Move({0,0,1});
	playerCapsule->Attach(gMainCam);
	playerCapsule->SetPos({0, 0, 2});
	
	auto ak47Graphics = gEngineCore.AddCompGraphicsFromFile(ak47ModelPath);
		ak47Graphics->SetScaleLocal({ 1.f / 1500, 1.f / 1500, 1.f / 1500 });
		ak47Graphics->SetRot(mm::quat(-0.05, {1,0,0}) * mm::quat(3.1415 - 0.08, { 0, 0, 1 }) * mm::quat(3.1415 / 2, { 1, 0, 0 })); // Need quaternion rework, multiply swapped, mm::quat(angle) rots with -angle, why?
		ak47Graphics->SetPos(gMainCam->GetPos() + mm::vec3(0.7, 1.05, -0.6));
		ak47Graphics->Rot(mm::quat(7 / 180.f * 3.1415, {0,0,1}) * mm::quat(-3 / 180.f * 3.1415, {1,0,0}));
	gMainCam->Attach(ak47Graphics); // Attach weapon to player physics


	// GAME CONSTANTS... TODO
	bool bMovingFront = false;
	bool bMovingBack = false;
	bool bMovingLeft = false;
	bool bMovingRight = false;
	const float playerMoveSpeed = 10;
	const float pixelsToRot360 = 1000;

	// Mouse recenter
	mm::vec2 windowCenter = gWindow->GetCenterPos();
	Sys::SetCursorPos(mm::uvec2((u32)windowCenter.x, (u32)windowCenter.y));
	
	while (gWindow->IsOpen())
	{
		float deltaSeconds = timer->GetSecondsPassed();
		timer->Reset();

		rWindowEvent evt;
		while (gWindow->PopEvent(&evt))
		{
			// W,S,A,D and Jump
			if(evt.msg == eWindowMsg::KEY_PRESS)
			{
				if(evt.key == eKey::SPACE)
					playerCapsule->AddForce({0,0,5100});
				if(evt.key == eKey::W)
					bMovingFront = true;
				if(evt.key == eKey::S)
					bMovingBack = true;
				if(evt.key == eKey::A)
					bMovingLeft = true;
				if(evt.key == eKey::D)
					bMovingRight = true;
			}
			else if(evt.msg == eWindowMsg::KEY_RELEASE)
			{
				if (evt.key == eKey::W)
					bMovingFront = false;
				if (evt.key == eKey::S)
					bMovingBack = false;
				if (evt.key == eKey::A)
					bMovingLeft = false;
				if (evt.key == eKey::D)
					bMovingRight = false;
			}
			// Look around with camera
			else if(evt.msg == eWindowMsg::MOUSE_MOVE)
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
				if(angleX * angleSign >= 3.14159265 / 2 * 0.95)
					angleX = 3.14159265 / 2 * 0.95 * angleSign;

				mm::quat rotAroundZ(angleZ, {0,0,1});
				mm::quat rotAroundX(angleX, {1,0,0});

				gMainCam->SetRot(rotAroundZ * rotAroundX);
			}
		}

		// Mouse recenter
		Sys::SetCursorPos(mm::uvec2((u32)windowCenter.x, (u32)windowCenter.y));

		// Player capsule move logic interpret
		mm::vec3 dMove(0,0,0);
		if(bMovingFront)
			dMove += gMainCam->GetDirFrontNormed() * deltaSeconds * playerMoveSpeed;
		if(bMovingBack)
			dMove += gMainCam->GetDirBackNormed() * deltaSeconds * playerMoveSpeed;
		if(bMovingLeft)
			dMove += gMainCam->GetDirLeftNormed() * deltaSeconds * playerMoveSpeed;
		if(bMovingRight)
			dMove += gMainCam->GetDirRightNormed() * deltaSeconds * playerMoveSpeed;
		dMove.z = 0;
		playerCapsule->Move(dMove);

		gEngineCore.Update(deltaSeconds);
		gWindow->Present();
	}
	return 0;
}