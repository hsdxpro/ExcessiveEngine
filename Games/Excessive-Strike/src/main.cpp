#include "..\Common\src\Factory.h"
#include "..\Common\src\IWindow.h"
#include "..\Common\src\Sys.h"
#include "..\Common\src\EngineCore.h"
#include "..\Common\src\ITimer.h"

// Spirit of the engine
EngineCore*			gEngineCore;
graphics::IEngine*	gEngineGraphics;
physics::IEngine*	gEnginePhysics;
sound::IEngine*		gEngineSound;

// Window for our game
IWindow* gWindow;

int main()
{
	// Full screen popup window for our game
	rWindow d;
		d.style = eWindowStyle::FULLSCREEN;
		d.clientW = Sys::getScreenSize().x;
		d.clientH = Sys::getScreenSize().y;
	gWindow = Factory::createWindow(d);

	gEngineCore = new EngineCore();
		rGraphicsEngineRaster graphicsDesc;
			graphicsDesc.gapiType = eGapiType::OPENGL_4_5;
			graphicsDesc.targetWindow = gWindow;
		gEngineGraphics = gEngineCore->initGraphicsEngineRaster(graphicsDesc);
		gEnginePhysics = gEngineCore->initPhysicsEngineBullet();
		gEngineSound = gEngineCore->initSoundEngine();

	ITimer* timer = Factory::createTimer();
	timer->start();

	// EZ A BULLSHIT RÉSZ AMIT ÁTKÉNE GONDOLNI HOGY LEGYEN
	{

		// Grafikus motor használatának nincs értelme 1 scene 1 layer nélkül, EngineCore majd elintézi
		auto mainScene = gEngineGraphics->createScene();
		graphics::IEngine::Layer layer;
		layer.scene = mainScene;
		gEngineGraphics->addLayer(layer);


		// Oké létrehozok kamerát, de azt majd core - n keresztül állítom be ????
		auto mainCam = gEngineGraphics->createCam();
		mainCam->setAspectRatio(gWindow->getClientAspectRatio());
		mainCam->setPos(mm::vec3(0, -3, 10));

		mainScene->setCamera(mainCam);

		// REMOVE THIS SHIIIIIT !!!!!! REFACTOR
		Actor* actorGround = gEngineCore->addActor(mainScene, Sys::getWorkDir() + L"../Assets/demo_ground.dae", 0);
		Actor* actorSky = gEngineCore->addActor(mainScene, Sys::getWorkDir() + L"../Assets/skybox.dae", 0);
		actorSky->setScale({ 1000, 1000, 1000 });
	}
	
	//Actor* a = gEngineCore->addActor();
	//	gEngineCore->addComponentGraphicsFromFile(a, modelPath);
	//	gEngineCore->addComponentRigidBodyFromFile(a, modelPath, 0);
	
	while (gWindow->isOpen())
	{
		rWindowEvent evt;
		while(gWindow->popEvent(&evt));

		float deltaSeconds = timer->getSecondsPassed();
		timer->reset();

		gEngineCore->update(deltaSeconds);
		gWindow->present();
	}
	return 0;
}