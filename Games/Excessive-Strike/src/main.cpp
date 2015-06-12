#include "..\Common\src\Factory.h"
#include "..\Common\src\IWindow.h"
#include "..\Common\src\Sys.h"
#include "..\Common\src\EngineCore.h"
#include "..\Common\src\ITimer.h"

// Spirit of the engine
EngineCore gEngineCore;

// Window for our game
IWindow* gWindow;

ComponentCamera* gCamComp;

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
	//gEngineCore.AddCompRigidBodyFromFile(groundModelPath, 0)->Attach(gEngineCore.AddCompGraphicsFromFile(groundModelPath));
	//gEngineCore.AddCompGraphicsFromFile(skyModelPath)->SetScale({ 1000, 1000, 1000 });

	//
	// Player components
	//auto compPhysics = gEngineCore.AddCompRigidBodyCapsule(2, 0.2, 20);
	gCamComp = gEngineCore.AddCompCamera();
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
	gEngineCore.SetCam(gCamComp);

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

				auto c0 = gEngineCore.AddCompGraphicsFromFile(boxModelPath);
				auto c1 = gEngineCore.AddCompGraphicsFromFile(boxModelPath);
				auto c2 = gEngineCore.AddCompGraphicsFromFile(boxModelPath);
				
				gCamComp->SetPos({0,-12, 5});
				
				c0->SetScale({ 1.f / 20, 1.f / 20, 1.f / 20 });
				c1->SetScale({ 1.f / 20, 1.f / 20, 1.f / 20 });
				c2->SetScale({ 1.f / 20, 1.f / 20, 1.f / 20 });
				
				
				c0->SetPos({ 2, -4, 6 });
				c1->SetPos({ -1, -4, 6 });
				c2->SetPos({ -1, -4, 3 });

				c0->SetRot(mm::quat(3.14159265 / 4, { 0, 1, 0 }));
				

				c0->Attach(c1);
				c1->Attach(c2);

				c0->Scale({ 2, 1, 2 });
				//c0->Scale({ 1, 1, 3 });

				//
				//c0->attachChild(c1);
				//
				//c0->SetScale(c1->GetScale() * 2);
				//c0->SetRot(mm::quat(3.14159265 / 4, { 0, 0, 1 }));



				//auto compPhysics = gEngineCore.AddCompRigidBodyFromFile(boxModelPath, 20);
				//compPhysics->SetScale({ 1.f / 20, 1.f / 20, 1.f / 20 });
				//compPhysics->SetPos({ 0, 0, 30.f });
				//
				//WorldComponent* boxes[1];
				//for (size_t i = 0; i < 1; i++)
				//{
				//	boxes[i] = gEngineCore.AddCompGraphicsFromFile(boxModelPath);
				//	boxes[i]->SetScale({ 1.f / 20, 1.f / 20, 1.f / 20 });
				//}
				//
				//mm::vec3 pos = compPhysics->GetPos();
				//const float dist = 2;
				//boxes[0]->SetPos(pos + mm::vec3(0, 0, 0));
				////boxes[1]->SetPos(pos + mm::vec3(0, 0, dist));
				////boxes[2]->SetPos(pos + mm::vec3(0, 0, -dist));
				////boxes[3]->SetPos(pos + mm::vec3(0, dist, 0));
				////boxes[4]->SetPos(pos + mm::vec3(0, -dist, 0));
				////boxes[5]->SetPos(pos + mm::vec3(dist, 0, 0));
				////boxes[6]->SetPos(pos + mm::vec3(-dist, 0, 0));
				//
				//compPhysics->attach(boxes[0]);
				////compPhysics->attach(boxes[1]);
				////compPhysics->attach(boxes[2]);
				////compPhysics->attach(boxes[3]);
				////compPhysics->attach(boxes[4]);
				////compPhysics->attach(boxes[5]);
				////compPhysics->attach(boxes[6]);
				//
				////gCamComp->detach();
				////gCamComp->detach();
				//gCamComp->detach();
				//gCamComp->SetPos(pos + mm::vec3(0, -13, 0));
				//
				////mm::quat asd = compPhysics->GetRot();
				////gCamComp->SetRot(asd);
				//
				//mm::vec3 dir = boxes[0]->GetPos() - gCamComp->GetPos();
				//	//dir.z = abs(dir.z + 0.2);
				//gCamComp->SetDirNormed(mm::normalize(dir));
				//
				//compPhysics->attach(gCamComp);
			}
		}

		float deltaSeconds = timer->GetSecondsPassed();
		timer->Reset();

		gEngineCore.Update(deltaSeconds);
		gWindow->Present();
	}
	return 0;
}