#include "tests.h"

// TMP for compiling
int Ricsi() {
	return 0;
}

/*
IGraphicsEngine* gEngine;
IGapi*			 gGapi;
IWindow*		 gWindow;
int Ricsi() {

	// Init graphics engine
	IGraphicsEngine::rDesc gDesc;
		gDesc....
		gDesc....
		gDesc....
	gEngine = IGraphicsEngine::Create(gDesc);
	gGapi = gEngine->GetGapi();

	// Init window
	IWindow::rDesc winDesc;
		winDesc.w = 800;
		winDesc.h = 600;
		winDesc.capText = "Excessive-Engine -> Ricsi teszt";
	gWindow = IWindow::Create(winDesc);


	IWindow::rEvent ev;
	while (gWindow->IsOpen())
	{
		while (gWindow->PopEvent(&msg))
			if (ev.msg == IWindow::eMsg::CLOSED || (msg == IWindow::eMsg::KEY_PRESSED && ev.key.code == ISystem::eKey::ESCAPE))
				gWindow->Close();

		gWindow->DisplayClientRect();
	}

	return 0;
}*/