#include "..\Common\src\Factory.h"
#include "..\Common\src\IWindow.h"
#include "..\Common\src\Sys.h"
#include "..\Common\src\EngineCore.h"

IWindow* gWindow;
EngineCore* gEngineCore;

int main()
{
	// Full screen popup window for our game
	rWindow d;
		d.clientW = Sys::getScreenSize().x;
		d.clientH = Sys::getScreenSize().y;
		d.style = eWindowStyle::FULLSCREEN;
	gWindow = Factory::createWindow(d);

	gEngineCore = new EngineCore();
		gEngineCore->initGraphicsEngineRaster({ eGapiType::OPENGL_4_5 });
		gEngineCore->initPhysicsEngineBullet();
		gEngineCore->initNetworkEngine();

	//ITimer* deltaTimer = Factory::createTimer();
	//deltaTimer->start();
	while (gWindow->isOpen())
	{
		rWindowEvent evt;
		while(gWindow->popEvent(&evt));

		//gEngineCore->update(deltaTimer->getSecondsSinceLastCall());
		gWindow->present();
	}
	return 0;
}