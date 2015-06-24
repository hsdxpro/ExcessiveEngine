#include "..\Common\src\Factory.h"
#include "..\Common\src\IWindow.h"
#include "..\Common\src\Sys.h"
#include "..\Common\src\ITimer.h"
#include "..\Common\src\EngineCpuProfiler.h"

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
		//d.style = eWindowStyle::FULLSCREEN;
		d.style = eWindowStyle::TITLE__RESIZE__CLOSE;
		d.clientW = 800;// Sys::GetScreenSize().x;
		d.clientH = 600;// Sys::GetScreenSize().y;
	gWindow = Factory::CreateWindow(d);

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
	gEngineCore.AddCompRigidBodyFromFile(groundModelPath, 0)->Attach(gEngineCore.AddCompGraphicsFromFile(groundModelPath));
	gEngineCore.AddCompGraphicsFromFile(skyModelPath)->SetScaleLocal({ 1000, 1000, 1000 });

	// Camera
	

	// Player components
	auto compPhysics = gEngineCore.AddCompRigidBodyCapsule(2, 0.2, 20);
		compPhysics->SetAngularFactor(0);
		compPhysics->SetKinematic();

	gMainCam = gEngineCore.AddCompCamera();
		compPhysics->Attach(gMainCam);
		compPhysics->SetPos({0, 0, 10});

	//auto compGraphics	= gEngineCore.AddCompGraphicsFromFile(ak47ModelPath);
	//	compGraphics->SetScale({ 1.f / 500, 1.f / 500, 1.f / 500 });
	//	//compGraphics->SetRot(mm::quat(-0.05, {1,0,0}) * mm::quat(3.1415 - 0.08, { 0, 0, 1 }) * mm::quat(3.1415 / 2, { 1, 0, 0 })); // Need quaternion rework, multiply swapped, mm::quat(angle) rots with -angle, why?
	////
	////// Build Player component tree
	//	gCamComp->SetPos({ 0.f, 0.f, 1.8f });
	//	//compGraphics->SetPos(gCamComp->GetPos() + mm::vec3(1.4, 2, -2.8));
	//
	////compPhysics->AddChild(compCamera);
	//gCamComp->SetPos({ 0, -8, 4 });
	//
	//compPhysics->attach(compGraphics);
	//compPhysics->attach(gCamComp);
	//
	//
	//compPhysics->SetPos({ 0.f, 0.f, 10.f });
	//
	gEngineCore.SetCam(gMainCam);

	//auto mat = gEngineCore.CreateGraphicsMaterial();
	//graphics::IMaterial::SubMaterial s;
	//	s.t_diffuse = gEngineCore.CreateGraphicsTexture();
	//	s.t_diffuse->load(Sys::GetWorkDir() + )
	//mat->AddSubMaterial()
	//comp->SetMaterial(mat);

	while (gWindow->IsOpen())
	{
		rWindowEvent evt;
		while (gWindow->PopEvent(&evt))
		{
			if (evt.msg == eWindowMsg::MOUSE_RELEASE && evt.mouseBtn == eMouseBtn::LEFT)
			{
				// Transform hierarchy UNIT TEST
				//auto c0 = gEngineCore.AddCompGraphicsFromFile(boxModelPath);
				//auto c1 = gEngineCore.AddCompGraphicsFromFile(boxModelPath);
				//auto c2 = gEngineCore.AddCompGraphicsFromFile(boxModelPath);
				//
				//gMainCam->SetPos({0,-25, 5});
				//
				//c0->SetScaleLocal({ 1.f / 20, 1.f / 20, 1.f / 20 });
				//c1->SetScaleLocal({ 1.f / 20, 1.f / 20, 1.f / 20 });
				//c2->SetScaleLocal({ 1.f / 20, 1.f / 20, 1.f / 20 });
				//
				//c0->SetPos({ 2, -4, 6 });
				//c1->SetPos({ -1, -4, 6 });
				//c2->SetPos({ -1, -4, 3 });
				//
				//
				//c0->SetRot(mm::quat(3.14159265 / 4, { 0, 1, 0 }));
				//c2->SetRot(mm::quat(3.14159265 / 4, { 0, 1, 0 }));
				//
				//
				//c0->Attach(c1);
				//c1->Attach(c2);
				//
				//c1->Rot(mm::quat(3.14159265 / 4, { 0, 1, 0 }));
				//
				//c0->ScaleLocal({ 1, 1, 3 });
				//
				//c1->Rot(mm::quat(3.14159265 / 4, { 0, 1, 0 }));
				//c1->Rot(mm::quat(3.14159265 / 4, { 0, 1, 0 }));
				//
				//// Ennek semmit se kéne csinálnia
				//c0->ScaleLocal({ 2, 1, 1 });
			}
		}

		float deltaSeconds = timer->GetSecondsPassed();
		timer->Reset();

		gEngineCore.Update(deltaSeconds);
		gWindow->Present();
	}
	return 0;
}