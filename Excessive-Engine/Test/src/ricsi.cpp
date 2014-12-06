#include "tests.h"
#include "Factory.h"
#include "IWindow.h"
#include "IGraphicsEngine.h"

ge::IGraphicsEngine*	gEngine;
ge::IGapi*				gGapi;
IWindow*				gWindow;

int Ricsi() {
	
	int asd = sizeof(size_t);

	// Init graphics engine
	ge::IGraphicsEngine::rCfg gDesc;

	gEngine = Factory::createGraphicsEngineRaster(gDesc);
	gGapi = gEngine->getGapi();

	//// Init window
	IWindow::rDesc winDesc;
		winDesc.clientW = 800;
		winDesc.clientH = 600;
		winDesc.capText = "Excessive-Engine -> Ricsi teszt";
	gWindow = Factory::createWindow(winDesc);

	
	IWindow::rEvent ev;
	while (gWindow->isOpen()) {
		while (gWindow->popEvent(&ev))
			if (ev.msg == IWindow::eMsg::KEY_PRESS && ev.key == Sys::eKey::ESCAPE)
				gWindow->close();
	
		// Update graphics engine
		gEngine->update();

		// Call that after OpenGL "finish" all of it's rendering
		gWindow->displayClientRect();
		
	}
	
	return 0;
}