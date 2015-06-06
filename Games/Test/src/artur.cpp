#include "EngineCore.h"
#include "Factory.h"

// basic util
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <sstream>
#include "Sys.h"

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
		IWindow* window = Factory::createWindow(d);

		// Init engine core (graphics, physics, sound, network
		EngineCore core;
		sound::IEngine* sEngine = core.initSoundEngine();
		core.initNetworkEngine();
		core.initPhysicsEngineBullet();
		rGraphicsEngineRaster gDesc;
		gDesc.gapiType = eGapiType::OPENGL_4_5;
		gDesc.targetWindow = window;
		graphics::IEngine* gEngine = core.initGraphicsEngineRaster(gDesc);

		// Create camera
		graphics::ICamera* cam = gEngine->createCam();
		cam->setFOV(70 / 180.f*3.1415926f);
		cam->setAspectRatio(window->getClientAspectRatio());
		cam->setNearPlane(0.2f);
		cam->setFarPlane(2000);
		cam->setPos(mm::vec3(0, -3, 1));

		// Create scene, plug camera
		graphics::IScene* scene = core.getDefaultGraphicsScene();
		scene->setCamera(cam);

		//graphics::IEngine::Layer layer;
		//layer.scene = scene;
		//gEngine->addLayer(layer);

		//*/
		static const wchar_t assetName[] = L"../Assets/demo_ground.dae"; // Assets/terminal/terminal.dae
		static const wchar_t teapotModelPath[] = L"../Assets/box.dae"; // Assets/teapot.dae
		/*/
		static const wchar_t assetName[] = L"../Assets/teapot.dae";
		//*/
		//Actor* simpleEntity = core.addActor();
		core.addCompRigidBodyFromFile(Sys::getWorkDir() + assetName, 0)->addChild(core.addCompGraphicsFromFile(Sys::getWorkDir() + assetName));

		core.addCompGraphicsFromFile(Sys::getWorkDir() + L"../Assets/skybox.dae")->setScale({ 1000, 1000, 1000 });

		auto pMusicSourceRepresenterModel = core.addCompGraphicsFromFile(Sys::getWorkDir() + teapotModelPath);
		pMusicSourceRepresenterModel->setScale({ 0.1f, 0.1f, 0.1f });
		pMusicSourceRepresenterModel->setPos(musicPosition);

		sound::IListener* listener = sEngine->createListener();
		listener->setUpwards(mm::vec3(0, 0, 1));
		listener->setDir(mm::vec3(0, 1, 0));
		sound::IScene* soundScene = sEngine->createScene();
		soundScene->setListener(listener);
		sound::IEmitter* pMusicSource = soundScene->addEmitter();
		sound::ISoundData* pMusicData = sEngine->createSoundData();
		auto musicFilePath = Sys::getWorkDir() + L"../Assets/PurgatorysMansion-mono.ogg";
		if (!pMusicData->load(musicFilePath.c_str(), sound::StoreMode::STREAMED)) {
			std::cout << "Failed to load: " << musicFilePath.c_str() << std::endl;
			return 1;
		}
		pMusicSource->setSoundData(pMusicData);
		pMusicSource->setPos(musicPosition);
		pMusicSource->setLooped(true);
		pMusicSource->start();
		sound::IEmitter* pFireSound = soundScene->addEmitter();
		sound::ISoundData* pFireSoundData = sEngine->createSoundData();
		auto fireSoundFilePath = Sys::getWorkDir() + L"../Assets/GUN_FIRE-stereo.ogg";
		if (!pFireSoundData->load(fireSoundFilePath.c_str(), sound::StoreMode::BUFFERED)) {
			std::cout << "Failed to load: " << fireSoundFilePath.c_str() << std::endl;
			return 1;
		}
		pFireSound->setSoundData(pFireSoundData);
		pFireSound->setVolume(0.3);
		//NOTE: FIRE SOUND IS STEREO, SFML IMPLEMENTATION CAN ONLY SPATIALIZE MONO SOUND. SO SETTING ITS POSITION IS MEANINGLESS.

		// Run the main loop
		rWindowEvent ev;
		double elapsed;
		std::chrono::time_point<std::chrono::high_resolution_clock> last_frame;

		// Timer for dT frame calc
		//ITimer* t = Factory::createTimer();

		bool bWDown = false;
		bool bSDown = false;
		bool bADown = false;
		bool bDDown = false;

		bool bRMBDown = false;

		last_frame = std::chrono::high_resolution_clock::now();

		while (window->isOpen())
		{
			//t->reset();

			// keep 60 fps
			auto now = std::chrono::high_resolution_clock::now();

			elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - last_frame).count() / 1.0e6;
			last_frame = now;

			int fps = 1 / elapsed + 0.5;
			std::wstringstream title_ss;
			static float camAngleX = 0; // upwards/downwards looking
			static float camAngleZ = 0; // orientation
			title_ss << L"Excessive-Engine - artur test | FPS=" << fps << L" | Pitch=" << camAngleX * 180 / 3.141592653f << L"° Facing=" << camAngleZ * 180 / 3.141592653f << L"°";
			window->setText(title_ss.str().c_str());

			while (window->popEvent(&ev))
				switch (ev.msg)
			{
				case eWindowMsg::MOUSE_PRESS:
					if (ev.mouseBtn == eMouseBtn::RIGHT)
						bRMBDown = true;
					else if (ev.mouseBtn == eMouseBtn::LEFT)
					{
						pFireSound->start();
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

						mm::vec3 viewDir = mm::normalize(cam->getTargetPos() - cam->getPos());
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

						cam->setTarget(cam->getPos() + newViewDir);
						listener->setDir(newViewDir);
					}
				} break;

				case eWindowMsg::KEY_PRESS:
					switch (ev.key)
					{
					case eKey::ESCAPE: window->close(); break;
					case eKey::W: bWDown = true; break;
					case eKey::S: bSDown = true; break;
					case eKey::A: bADown = true; break;
					case eKey::D: bDDown = true; break;
					case eKey::LSHIFT: gCamSpeedMultiplier = 5; break;
					case eKey::SPACE: {
						musicPosition = cam->getPos() + cam->getDirFront() * 3;
						pMusicSourceRepresenterModel->setPos(musicPosition);
						pMusicSource->setPos(musicPosition);
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
					cam->setAspectRatio((float)ev.x / (float)ev.y);
					break;
			}

			// Camera move
			if (bWDown) // W
				cam->setPos(cam->getPos() + cam->getDirFront() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
			if (bSDown) // S									 
				cam->setPos(cam->getPos() + cam->getDirBack()  * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
			if (bADown) // A									 
				cam->setPos(cam->getPos() + cam->getDirLeft()  * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
			if (bDDown) // D									 
				cam->setPos(cam->getPos() + cam->getDirRight() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);

			listener->setPos(cam->getPos());
			// Update everything

			//float deltaT = t->getElapsedSinceReset();

			// Update core
			core.update(elapsed/*, scene*/);

			// Call that after OpenGL "finish" all of it's rendering
			window->present();

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
