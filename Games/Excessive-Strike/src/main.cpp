#include "..\Common\src\Factory.h"
#include "..\Common\src\IWindow.h"
#include "..\Common\src\Sys.h"
#include "..\Common\src\EngineCore.h"
#include "..\Common\src\ITimer.h"

// Spirit of the engine
EngineCore gEngineCore;

// Window for our game
IWindow* gWindow;

int main()
{
	// Full screen popup window for our game
	rWindow d;
		//d.style = eWindowStyle::FULLSCREEN;
		d.style = eWindowStyle::TITLE__RESIZABLE__MAXIMIZEBUTTON;
		d.clientW = 800;// Sys::getScreenSize().x;
		d.clientH = 600;// Sys::getScreenSize().y;
	gWindow = Factory::createWindow(d);

	// Init Engine core
	rGraphicsEngineRaster graphicsDesc;
		graphicsDesc.gapiType = eGapiType::OPENGL_4_5;
		graphicsDesc.targetWindow = gWindow;
	gEngineCore.initGraphicsEngineRaster(graphicsDesc);
	gEngineCore.initPhysicsEngineBullet();
	gEngineCore.initSoundEngine();

	ITimer* timer = Factory::createTimer();
	timer->start();

	// Create camera
	auto cam = gEngineCore.getGraphicsEngine()->createCam();
		cam->setAspectRatio(gWindow->getClientAspectRatio());
		cam->setPos(mm::vec3(0, -3, 10));
	gEngineCore.getDefaultGraphicsScene()->setCamera(cam);

	// Add some actors
	auto& groundModelPath = Sys::getWorkDir() + L"../Assets/demo_ground.dae";
	auto& skyModelPath = Sys::getWorkDir() + L"../Assets/skybox.dae";
	gEngineCore.addCompRigidBodyFromFile(groundModelPath, 0)->addCompGraphicsFromFile(groundModelPath);
	gEngineCore.addCompGraphicsFromFile(skyModelPath)->getRootComp<ComponentGraphics>()->setScale({ 1000, 1000, 1000 });

	while (gWindow->isOpen())
	{
		rWindowEvent evt;
		while(gWindow->popEvent(&evt));

		float deltaSeconds = timer->getSecondsPassed();
		timer->reset();

		gEngineCore.update(deltaSeconds);
		gWindow->present();
	}
	return 0;
}