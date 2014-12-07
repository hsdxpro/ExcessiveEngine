#include "tests.h"
#include "Factory.h"
#include "IWindow.h"
#include "IGraphicsEngine.h"

ge::IGraphicsEngine*	gEngine;
ge::IGapi*				gGapi;
IWindow*				gWindow;

int Ricsi() {

	// Init GraphicsEngine
	ge::rGraphicsEngine gDesc;
		gDesc.gapi = Factory::createGapiGL();
	gEngine = Factory::createGraphicsEngineRaster(gDesc);

	// Get the API belonging to graphics engine
	gGapi = gEngine->getGapi();

	//// Init window
	rWindow d;
		d.clientW = 800;
		d.clientH = 600;
		d.capText = "Excessive-Engine -> Ricsi teszt";
	gWindow = Factory::createWindow(d);

	
	rWindowEvent ev;
	while (gWindow->isOpen()) {
		while (gWindow->popEvent(&ev))
			if (ev.msg == eWindowMsg::KEY_PRESS && ev.key == eKey::ESCAPE)
				gWindow->close();

		// Update graphics engine
		gEngine->update();

		// Call that after OpenGL "finish" all of it's rendering
		gWindow->displayClientRect();
		
	}
	
	return 0;
}