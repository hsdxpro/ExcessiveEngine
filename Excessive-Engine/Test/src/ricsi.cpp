#include "tests.h"
#include "..\..\GraphicsEngine_Interface\interface\IGraphicsEngine.h"
#include "..\..\GraphicsApi_Interface\interface\IGapi.h"

ge::IGraphicsEngine* gEngine;
ge::IGapi*				 gGapi;
//IWindow*		 gWindow;
int Ricsi() {
	return 0;
	
	//// Init graphics engine
	ge::IGraphicsEngine::rDesc gDesc;
		//gDesc....
		//gDesc....
		//gDesc....
	//gEngine = ge::IGraphicsEngine::create(gDesc);
	//gGapi = gEngine->getGapi();
	//
	//// Init window
	//IWindow::rDesc winDesc;
	//	winDesc.w = 800;
	//	winDesc.h = 600;
	//	winDesc.capText = "Excessive-Engine -> Ricsi teszt";
	//gWindow = IWindow::Create(winDesc);
	//
	//
	//IWindow::rEvent ev;
	//while (gWindow->IsOpen())
	//{
	//	while (gWindow->PopEvent(&msg))
	//		if (ev.msg == IWindow::eMsg::CLOSED || (msg == IWindow::eMsg::KEY_PRESSED && ev.key.code == ISystem::eKey::ESCAPE))
	//			gWindow->Close();
	//
	//	gWindow->DisplayClientRect();
	//}
	//
	//return 0;
}