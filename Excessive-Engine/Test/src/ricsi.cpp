#include "tests.h"
#include "Factory.h"
#include "IWindow.h"
#include "IGraphicsEngine.h"

ge::IGraphicsEngine*	gEngine;
ge::IGapi*				gGapi;
IWindow*					gWindow;

int Ricsi() {
	
	
	// Init graphics engine
	gEngine = Factory::createGraphicsEngineRaster();
	
	ge::IGraphicsEngine::rDesc gDesc;
	//	gDesc....
	//	gDesc...
	//gEngine->init(gDesc);
	gGapi = gEngine->getGapi();

	//// Init window
	IWindow::rDesc winDesc;
		winDesc.clientW = 800;
		winDesc.clientH = 600;
		winDesc.capText = L"Excessive-Engine -> Ricsi teszt";
	//gWindow = new Window(winDesc);

	
	IWindow::rEvent ev;
	while (gWindow->isOpen())
	{
		while (gWindow->popEvent(&ev))
			if (ev.msg == IWindow::eMsg::CLOSED || (ev.msg == IWindow::eMsg::KEY_PRESSED && ev.key == Sys::eKey::ESCAPE))
				gWindow->close();
	
		// Update graphics engine
		gEngine->update();

		// Call that after OpenGL "finish" all of it's rendering
		gWindow->displayClientRect();
		
	}
	
	return 0;
}