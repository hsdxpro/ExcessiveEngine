#include "tests.h"
//#include "..\..\GraphicsEngine_Interface\interface\IGraphicsEngine.h"
//#include "..\..\GraphicsApi_Interface\interface\IGapi.h"
#include "Factory.h"

ge::IGraphicsEngine*	gEngine;
ge::IGapi*				gGapi;
IWindow*				gWindow;

int Ricsi() {
	
	// Init graphics engine
	ge::IGraphicsEngine::rDesc gDesc;

	gEngine = Factory::createGraphicsEngine(gDesc);
	//gGapi = gEngine->getGapi();

	//// Init window
	IWindow::rDesc winDesc;
		winDesc.clientW = 800;
		winDesc.clientH = 600;
		winDesc.capText = L"Excessive-Engine -> Ricsi teszt";
	gWindow = Factory::createWindow(winDesc);

	
	IWindow::rEvent ev;
	while (gWindow->isOpen())
	{
		while (gWindow->PopEvent(&ev))
			if (ev.msg == IWindow::eMsg::CLOSED || (ev.msg == IWindow::eMsg::KEY_PRESSED && ev.key == ISys::eKey::ESCAPE))
				gWindow->close();
	
		gWindow->displayClientRect();
	}
	
	return 0;
}