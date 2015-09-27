#include "Core\EngineCore.h"
#include "Core\InputCore.h"
#include "Core\WorldCore.h"
#include "Core\CameraComponent.h"

#include "PlatformLibrary\Window.h"
#include "PlatformLibrary\Timer.h"
#include "PlatformLibrary\File.h"

#include "SupportLibrary\VisualCpuProfiler.h"
#include <sstream>

namespace testartur
{
	const float CAM_MOVE_SPEED = 5;
	float gCamSpeedMultiplier = 1;

	//IGapi* gGapi;

	int artur_main()
	{
		mm::vec3 musicPosition(0, 10, 0);

		// Init window
		rWindow d;
		d.clientW = 800;
		d.clientH = 600;
		d.capText = "Excessive-Engine -> artur test";
		Window window(d);

		// Init engine core (graphics, physics, sound, network
		ISoundEngine* sEngine = Core.InitSoundEngineSFML();
		Core.InitNetworkEngine();
		Core.InitPhysicsEngineBullet();
		rGraphicsEngineRaster gDesc;
		gDesc.gapiType = eGapiType::OPENGL_4_5;
		gDesc.targetWindow = &window;
		IGraphicsEngine* gEngine = Core.InitGraphicsEngineRaster(gDesc);

		// Create camera
		CameraComponent* cam = World.SpawnComp_Camera();
		cam->SetFOV(70 / 180.f*3.1415926f);
		//cam->SetAspectRatio(window->GetClientAspectRatio());
		cam->SetNearPlane(0.2f);
		cam->SetFarPlane(2000);
		cam->SetPos(mm::vec3(0, -3, 1));

		Core.SetCam(cam);

		//IGraphicsEngine::Layer layer;
		//layer.scene = scene;
		//gEngine->AddLayer(layer);

		//*/
		static const char assetName[] = "demo_ground.dae"; // Assets/terminal/terminal.dae
		static const char teapotModelPath[] = "box.dae"; // Assets/teapot.dae
		/*/
		static const wchar_t assetName[] = L"teapot.dae";
		//*/
		//Entity* simpleEntity = Core.AddActor();
		World.SpawnComp_RigidBodyFromFile(assetName, 0)->Attach(Core.SpawnComp_MeshFromFile(assetName));

		World.SpawnComp_MeshFromFile("skybox.dae")->SetScaleLocal({ 1000, 1000, 1000 });

		auto pMusicSourceRepresenterModel = World.SpawnComp_MeshFromFile(teapotModelPath);
		pMusicSourceRepresenterModel->SetScaleLocal({ 0.1f, 0.1f, 0.1f });
		pMusicSourceRepresenterModel->SetPos(musicPosition);

		//sound::IListener* listener = sEngine->CreateListener();
		//listener->SetUpwards(mm::vec3(0, 0, 1));
		//listener->SetDir(mm::vec3(0, 1, 0));
		sound::IScene* soundScene = sEngine->CreateScene();
		//soundScene->SetListener(listener);
		//sound::IEmitter* pMusicSource = soundScene->AddEmitter();
		//sound::ISoundData* pMusicData = sEngine->CreateSoundData();
		//auto musicFilePath = Sys::GetWorkDirW() + L"PurgatorysMansion-mono.ogg";
		//if (!pMusicData->Load(musicFilePath.c_str(), sound::StoreMode::STREAMED)) {
		//	std::cout << "Failed to load: " << musicFilePath.c_str() << std::endl;
		//	return 1;
		//}
		//pMusicSource->SetSoundData(pMusicData);
		//pMusicSource->SetPos(musicPosition);
		//pMusicSource->SetLooped(true);
		//pMusicSource->Start();

		sound::IEmitter* pFireSound = soundScene->AddEmitter();
		sound::ISoundData* pFireSoundData = sEngine->CreateSoundData();
		auto fireSoundFilePath = "GUN_FIRE-stereo.ogg";
		if (!pFireSoundData->Load(fireSoundFilePath, sound::StoreMode::BUFFERED)) {
			std::cout << "Failed to load: " << fireSoundFilePath << std::endl;
			return 1;
		}
		pFireSound->SetSoundData(pFireSoundData);
		pFireSound->SetVolume(0.3f);
		//NOTE: FIRE SOUND IS STEREO, SFML IMPLEMENTATION CAN ONLY SPATIALIZE MONO SOUND. SO SETTING ITS POSITION IS MEANINGLESS.

		// Run the main loop
		rWindowEvent ev;
		double elapsed;
		std::chrono::time_point<std::chrono::high_resolution_clock> last_frame;

		// Timer for dT frame calc
		//ITimer* t = Factory::CreateTimer();

		bool bWDown = false;
		bool bSDown = false;
		bool bADown = false;
		bool bDDown = false;

		bool bRMBDown = false;

		last_frame = std::chrono::high_resolution_clock::now();

		while (window.IsOpen())
		{
			//t->Reset();

			// keep 60 fps
			auto now = std::chrono::high_resolution_clock::now();

			elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - last_frame).count() / 1.0e6;
			last_frame = now;

			int fps = 1 / elapsed + 0.5;
			std::stringstream title_ss;
			static float camAngleX = 0; // upwards/downwards looking
			static float camAngleZ = 0; // orientation
			title_ss << L"Excessive-Engine - artur test | FPS=" << fps << L" | Pitch=" << camAngleX * 180 / 3.141592653f << L"° Facing=" << camAngleZ * 180 / 3.141592653f << L"°";
			window.SetTitle(title_ss.str().c_str());

			while (window.PopEvent(ev))
				switch (ev.msg)
			{
				case eWindowMsg::MOUSE_PRESS:
					if (ev.mouseBtn == eMouseBtn::RIGHT)
						bRMBDown = true;
					else if (ev.mouseBtn == eMouseBtn::LEFT)
					{
						pFireSound->Start();
					}
					break;
				case eWindowMsg::MOUSE_RELEASE:
					if (ev.mouseBtn == eMouseBtn::RIGHT)
						bRMBDown = false;
					break;
				case eWindowMsg::MOUSE_MOVE:
				{
					if (bRMBDown)
					{
						static float angleZ = 0;
						static float angleX = 0;
						
						// Input read up finished, now we can recenter cursor for our fps game
						auto mousePos = Sys::GetCursorPos();
						
						//float mouseDx = mousePos.x - windowCenter.x;
						//float mouseDy = mousePos.y - windowCenter.y;
						
						angleZ += -(float)ev.deltaX / 360.f * 6.28;
						angleX += -(float)ev.deltaY / 360.f * 6.28;
						
						// Clamp angleX
						float angleSign = angleX >= 0 ? 1 : -1;
						if (angleX * angleSign >= 3.14159265 / 2 * 0.95)
							angleX = 3.14159265 / 2 * 0.95 * angleSign;
						
						mm::quat rotAroundZ(angleZ, { 0, 0, 1 });
						mm::quat rotAroundX(angleX, { 1, 0, 0 });
						
						cam->SetRot(rotAroundZ * rotAroundX);
					}
				} break;

				case eWindowMsg::KEY_PRESS:
					switch (ev.key)
					{
					case eKey::ESCAPE: window.Close(); break;
					case eKey::W: bWDown = true; break;
					case eKey::S: bSDown = true; break;
					case eKey::A: bADown = true; break;
					case eKey::D: bDDown = true; break;
					case eKey::LSHIFT: gCamSpeedMultiplier = 5; break;
					case eKey::SPACE: {
						musicPosition = cam->GetPos() + cam->GetFrontDirNormed() * 3;
						pMusicSourceRepresenterModel->SetPos(musicPosition);
						//pMusicSource->SetPos(musicPosition);
					} break;

					} break;
				case eWindowMsg::KEY_RELEASE:
					switch (ev.key)
					{
					case eKey::W: bWDown = false; break;
					case eKey::S: bSDown = false; break;
					case eKey::A: bADown = false; break;
					case eKey::D: bDDown = false; break;
					case eKey::LSHIFT: gCamSpeedMultiplier = 1; break;
					} break;

				case eWindowMsg::RESIZE:
					//cam->SetAspectRatio((float)ev.x / (float)ev.y);
					break;
			}

			// Camera move
			if (bWDown) // W
				cam->SetPos(cam->GetPos() + cam->GetFrontDirNormed() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
			if (bSDown) // S									 
				cam->SetPos(cam->GetPos() + cam->GetBackDirNormed()  * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
			if (bADown) // A									 
				cam->SetPos(cam->GetPos() + cam->GetLeftDirNormed()  * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
			if (bDDown) // D									 
				cam->SetPos(cam->GetPos() + cam->GetRightDirNormed() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);

//			listener->SetPos(cam->GetPos());
			// Update everything

			//float deltaT = t->GetElapsedSinceReset();

			// Update core
			Core.Update(elapsed/*, scene*/);

			// Call that after OpenGL "finish" all of it's rendering
			//window.Present();

			// keep 60 fps
			//std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
			//std::chrono::microseconds sleep_time = std::chrono::microseconds(16667) - std::chrono::duration_cast<std::chrono::microseconds>(now - last_frame);
			//this_thread::sleep_for(sleep_time);
		}
		std::cout << std::endl;

		return 0;
	}
}

int artur()
{
	return testartur::artur_main();
}
