#include <Core/EngineCore.h>
#include <SupportLibrary/Factory.h>

// basic util
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <sstream>
#include <PlatformLibrary/Sys.h>
#include <PlatformLibrary/Window.h>

namespace testartur
{
	const float CAM_MOVE_SPEED = 5;
	float gCamSpeedMultiplier = 1;

	IGapi* gGapi;

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
		EngineCore core;
		sound::IEngine* sEngine = core.InitSoundEngine();
		core.InitNetworkEngine();
		core.InitPhysicsEngineBullet();
		rGraphicsEngineRaster gDesc;
		gDesc.gapiType = eGapiType::OPENGL_4_5;
		gDesc.targetWindow = &window;
		graphics::IEngine* gEngine = core.InitGraphicsEngineRaster(gDesc);

		// Create camera
		graphics::ICamera* cam = gEngine->CreateCam();
		cam->SetFOV(70 / 180.f*3.1415926f);
		//cam->SetAspectRatio(window->GetClientAspectRatio());
		cam->SetNearPlane(0.2f);
		cam->SetFarPlane(2000);
		cam->SetPos(mm::vec3(0, -3, 1));

		// Create scene, plug camera
		graphics::IScene* scene = core.GetDefaultGraphicsScene();
		scene->SetCamera(cam);

		//graphics::IEngine::Layer layer;
		//layer.scene = scene;
		//gEngine->AddLayer(layer);

		//*/
		static const wchar_t assetName[] = L"../Assets/demo_ground.dae"; // Assets/terminal/terminal.dae
		static const wchar_t teapotModelPath[] = L"../Assets/box.dae"; // Assets/teapot.dae
		/*/
		static const wchar_t assetName[] = L"../Assets/teapot.dae";
		//*/
		//Actor* simpleEntity = core.AddActor();
		core.AddCompRigidBodyFromFile(Sys::GetWorkDir() + assetName, 0)->Attach(core.AddCompGraphicsFromFile(Sys::GetWorkDir() + assetName));

		core.AddCompGraphicsFromFile(Sys::GetWorkDir() + L"../Assets/skybox.dae")->SetScaleLocal({ 1000, 1000, 1000 });

		auto pMusicSourceRepresenterModel = core.AddCompGraphicsFromFile(Sys::GetWorkDir() + teapotModelPath);
		pMusicSourceRepresenterModel->SetScaleLocal({ 0.1f, 0.1f, 0.1f });
		pMusicSourceRepresenterModel->SetPos(musicPosition);

		sound::IListener* listener = sEngine->CreateListener();
		listener->SetUpwards(mm::vec3(0, 0, 1));
		listener->SetDir(mm::vec3(0, 1, 0));
		sound::IScene* soundScene = sEngine->CreateScene();
		soundScene->SetListener(listener);
		sound::IEmitter* pMusicSource = soundScene->AddEmitter();
		sound::ISoundData* pMusicData = sEngine->CreateSoundData();
		auto musicFilePath = Sys::GetWorkDir() + L"../Assets/PurgatorysMansion-mono.ogg";
		if (!pMusicData->Load(musicFilePath.c_str(), sound::StoreMode::STREAMED)) {
			std::cout << "Failed to load: " << musicFilePath.c_str() << std::endl;
			return 1;
		}
		pMusicSource->SetSoundData(pMusicData);
		pMusicSource->SetPos(musicPosition);
		pMusicSource->SetLooped(true);
		pMusicSource->Start();
		sound::IEmitter* pFireSound = soundScene->AddEmitter();
		sound::ISoundData* pFireSoundData = sEngine->CreateSoundData();
		auto fireSoundFilePath = Sys::GetWorkDir() + L"../Assets/GUN_FIRE-stereo.ogg";
		if (!pFireSoundData->Load(fireSoundFilePath.c_str(), sound::StoreMode::BUFFERED)) {
			std::cout << "Failed to load: " << fireSoundFilePath.c_str() << std::endl;
			return 1;
		}
		pFireSound->SetSoundData(pFireSoundData);
		pFireSound->SetVolume(0.3);
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
			std::wstringstream title_ss;
			static float camAngleX = 0; // upwards/downwards looking
			static float camAngleZ = 0; // orientation
			title_ss << L"Excessive-Engine - artur test | FPS=" << fps << L" | Pitch=" << camAngleX * 180 / 3.141592653f << L"° Facing=" << camAngleZ * 180 / 3.141592653f << L"°";
			window.SetTitle(title_ss.str().c_str());

			while (window.PopEvent(&ev))
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
						float angleChangeZ = (float)(ev.deltaX) * 0.009;
						float angleChangeX = (float)(-ev.deltaY) * 0.009;

						mm::vec3 viewDir = mm::normalize(cam->GetTargetPos() - cam->GetPos());
						float lenXY = mm::length(viewDir.xy);
						static float angleX = acos(lenXY)*(viewDir.z > 0 ? 1 : -1);
						angleX += angleChangeX;
						angleX = std::max(-85.f / 180 * 3.141592653f, std::min(angleX, 85.f / 180 * 3.141592653f));
						static float angleZ = atan2(viewDir.y / lenXY, viewDir.z / lenXY) - 3.141592653f / 2;
						angleZ += angleChangeZ;
						if (angleZ > 3.141592653f)
							angleZ -= floor(angleZ / 3.141592653f) * 2 * 3.141592653f;
						else if (angleZ < -3.141592653f)
							angleZ -= ceil(angleZ / 3.141592653f) * 2 * 3.141592653f;

						mm::vec3 newViewDir(0, 1, 0);

						mm::mat3 rotAroundX(1, 0, 0,
							0, cos(angleX), -sin(angleX),
							0, sin(angleX), cos(angleX));

						mm::mat3 rotAroundZ(cos(-angleZ), -sin(-angleZ), 0,
							sin(-angleZ), cos(-angleZ), 0,
							0, 0, 1);

						newViewDir *= rotAroundX;
						newViewDir *= rotAroundZ;

						camAngleX = angleX;
						camAngleZ = angleZ;

						cam->SetTarget(cam->GetPos() + newViewDir);
						listener->SetDir(newViewDir);
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
						musicPosition = cam->GetPos() + cam->GetDirFrontNormed() * 3;
						pMusicSourceRepresenterModel->SetPos(musicPosition);
						pMusicSource->SetPos(musicPosition);
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
				cam->SetPos(cam->GetPos() + cam->GetDirFrontNormed() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
			if (bSDown) // S									 
				cam->SetPos(cam->GetPos() + cam->GetDirBackNormed()  * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
			if (bADown) // A									 
				cam->SetPos(cam->GetPos() + cam->GetDirLeftNormed()  * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
			if (bDDown) // D									 
				cam->SetPos(cam->GetPos() + cam->GetDirRightNormed() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);

			listener->SetPos(cam->GetPos());
			// Update everything

			//float deltaT = t->GetElapsedSinceReset();

			// Update core
			core.Update(elapsed/*, scene*/);

			// Call that after OpenGL "finish" all of it's rendering
			window.Present();

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
