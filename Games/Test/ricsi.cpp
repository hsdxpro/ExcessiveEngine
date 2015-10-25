//#include "Core/EngineCore.h"
//#include "Core/InputCore.h"
//#include "Core/GraphicsCore.h"
//#include "Core/EngineWorld.h"
//#include "Core/CameraComponent.h"
//
//#include "PlatformLibrary\Window.h"
//#include "PlatformLibrary\Timer.h"
//#include "PlatformLibrary\File.h"
//
//#include "SupportLibrary\VisualCpuProfiler.h"
//#include <sstream>
//
//#define CAM_MOVE_SPEED 5
//float gCamSpeedMultiplier = 1;
//
//IGapi* gGapi;
//
//int Ricsi() 
//{
//	// Init window
//	rWindow d;
//		d.clientW = 800;
//		d.clientH = 600;
//		d.capText = "Excessive-Engine -> Ricsi teszt";
//	Window window(d);
//
//	// Init engine core (graphics, physics, sound, network
//	Core.InitSoundEngineSFML();
//	Core.InitNetworkEngine();
//	Core.InitPhysicsEngineBullet();
//		rGraphicsEngineRaster gDesc;
//		gDesc.gapiType = eGapiType::OPENGL_4_5;
//		gDesc.targetWindow = &window;
//	IGraphicsEngine* gEngine = Core.InitGraphicsEngineRaster(gDesc);
//
//	// Create camera
//	CameraComponent* cam = World.SpawnComp_Camera();
//		cam->SetFOV(70 / 180.f*3.1415926f);
//		cam->SetNearPlane(0.2f);
//		cam->SetFarPlane(2000);
//		cam->SetPos(mm::vec3(0, -3, 1));
//	Graphics.SetCam(cam);
//
//	static const char assetName[] = "demo_ground.dae"; // Assets/terminal/terminal.dae
//	static const char teapotModelPath[] = "box.dae"; // Assets/teapot.dae
//	static const char ak47ModelPath[] = "ak47/ak.obj"; // Assets/teapot.dae
//
//	World.SpawnComp_RigidBodyFromFile(assetName, 0)->Attach(World.SpawnComp_MeshFromFile(assetName));
//	World.SpawnComp_MeshFromFile("skybox.dae")->SetScaleLocal({ 1000, 1000, 1000 });
//
//	// Run the main loop
//	rWindowEvent ev;
//	double elapsed;
//	double last_frame;
//
//	// Timer for dT frame calc
//	Timer timer;
//	timer.Start();
//	last_frame = 0;
//	elapsed = 0;
//
//	bool bWDown = false;
//	bool bSDown = false;
//	bool bADown = false;
//	bool bDDown = false;
//
//	bool bRMBDown = false;
//
//	// Jesus, just because we dont want garbage for last_frame variable at first run
//	static bool bFirstRun = true;
//
//	while (window.IsOpen())
//	{
//		// keep 60 fps
//		double now = timer.GetSecondsPassed();
//
//		if (bFirstRun) 
//		{
//			bFirstRun = false;
//			last_frame = now;
//		}
//		elapsed = now - last_frame;
//		last_frame = now;
//
//		int fps = 1 / elapsed + 0.5;
//		std::stringstream title_ss;
//		static float camAngleX = 0; // upwards/downwards looking
//		static float camAngleZ = 0; // orientation
//		title_ss << L"Excessive-Engine - Ricsi teszt | FPS=" << fps << L" | Pitch=" << camAngleX * 180 / 3.141592653f << L"° Facing=" << camAngleZ * 180 / 3.141592653f << L"°";
//		window.SetTitle(title_ss.str().c_str());
//
//		while (window.PopEvent(ev))
//		switch (ev.msg)
//		{
//			case eWindowMsg::MOUSE_PRESS:
//				if (ev.mouseBtn == eMouseBtn::RIGHT)
//					bRMBDown = true;
//				else if (ev.mouseBtn == eMouseBtn::LEFT)
//				{
//					auto box = World.SpawnComp_RigidBodyFromFile(teapotModelPath, 10);
//					box->Attach(World.SpawnComp_MeshFromFile(teapotModelPath));
//
//					box->SetPos(cam->GetPos() + cam->GetCam()->GetFrontDirNormed() * 3); // 3 méterrel elénk
//					box->SetScaleLocal(mm::vec3(1.f / 20, 1.f / 20, 1.f / 20));
//				}
//				break;
//			case eWindowMsg::MOUSE_RELEASE:
//				if (ev.mouseBtn == eMouseBtn::RIGHT)
//					bRMBDown = false;
//				break;
//			case eWindowMsg::MOUSE_MOVE: 
//			{
//				if (bRMBDown)
//				{
//					float angleChangeZ = (float)(ev.deltaX) * 0.009;
//					float angleChangeX = (float)(-ev.deltaY) * 0.009;
//
//					mm::vec3 viewDir = mm::normalize(cam->GetCam()->GetTargetPos() - cam->GetPos());
//					float lenXY = mm::length(viewDir.xy);
//					static float angleX = acos(lenXY)*(viewDir.z > 0 ? 1 : -1);
//					angleX += angleChangeX;
//					angleX = std::max(-85.f / 180 * 3.141592653f, std::min(angleX, 85.f / 180 * 3.141592653f));
//					static float angleZ = atan2(viewDir.y / lenXY, viewDir.z / lenXY) - 3.141592653f / 2;
//					angleZ += angleChangeZ;
//					if (angleZ > 3.141592653f)
//						angleZ -= floor(angleZ / 3.141592653f) * 2 * 3.141592653f;
//					else if (angleZ < -3.141592653f)
//						angleZ -= ceil(angleZ / 3.141592653f) * 2 * 3.141592653f;
//
//					mm::vec3 newViewDir(0, 1, 0);
//
//					// MY HEKK
//					angleZ = 3.1415f / 2;
//
//					mm::mat3 rotAroundX(1,		0,			0,
//										0, cos(angleX), -sin(angleX),
//										0, sin(angleX),  cos(angleX));
//
//					mm::mat3 rotAroundZ(cos(-angleZ), -sin(-angleZ),	0,
//										sin(-angleZ),  cos(-angleZ),	0,
//											0,			0,				1);
//
//					newViewDir *= rotAroundX;
//					newViewDir *= rotAroundZ;
//
//					camAngleX = angleX;
//					camAngleZ = angleZ;
//
//					cam->SetDirNormed(mm::normalize(newViewDir));
//					//cam->SetTarget(cam->GetPos() + newViewDir);
//				}
//			} break;
//
//			case eWindowMsg::KEY_PRESS:
//				switch (ev.key)
//				{
//					case eKey::ESCAPE: window.Close(); break;
//					case eKey::W: bWDown = true; break;
//					case eKey::S: bSDown = true; break;
//					case eKey::A: bADown = true; break;
//					case eKey::D: bDDown = true; break;
//					case eKey::LSHIFT: gCamSpeedMultiplier = 5; break;
//				} break;
//			case eWindowMsg::KEY_RELEASE:
//				switch (ev.key)
//				{
//					case eKey::W: bWDown = false; break;
//					case eKey::S: bSDown = false; break;
//					case eKey::A: bADown = false; break;
//					case eKey::D: bDDown = false; break;
//					case eKey::LSHIFT: gCamSpeedMultiplier = 1; break;
//				} break;
//
//			case eWindowMsg::RESIZE:
//				//cam->SetAspectRatio((float)ev.x / (float)ev.y);
//				break;
//		}
//
//		// Camera move
//		if (bWDown) // W
//			cam->SetPos(cam->GetPos() + cam->GetCam()->GetFrontDirNormed() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
//		if (bSDown) // S									 
//			cam->SetPos(cam->GetPos() + cam->GetCam()->GetBackDirNormed()  * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
//		if (bADown) // A									 
//			cam->SetPos(cam->GetPos() + cam->GetCam()->GetLeftDirNormed()  * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
//		if (bDDown) // D									 
//			cam->SetPos(cam->GetPos() + cam->GetCam()->GetRightDirNormed() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
//
//		// Update core
//		Core.Update(elapsed/*, scene*/);
//	}
//	std::cout << std::endl;
//
//	return 0;
//}