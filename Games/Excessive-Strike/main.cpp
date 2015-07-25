// Ismét tisztázzuk mostantól kezdve mire is van szükségünk, haladjunk szükségleti sorrendben...

//1. Szeretnék egy script - bõl kattintásra lõni egy adott irányba egy grafikát, ami magától 200 méter után azt mondja hogy törlõdjön ki
//
//2. Szeretnék unit test - et írni a "Mutated Game Scripting" - re, de egy elég durvát
//3. Próbálj találni végre olyan játék logikai dolgot amire hasznos lehet a WorldComponent mutálás, nem csak az Entity mutálás...

#include "Core\Core.h"
#include "PlatformLibrary\IWindow.h"
#include "Core\CameraComponent.h"
#include "PlatformLibrary\Window.h"
#include "PlatformLibrary\Timer.h"
#include "SupportLibrary\VisualCpuProfiler.h"
#include "PlayerScript.h"
#include "TestLevelScript.h"
#include "Core\Input.h"

void InitScript();

int main()
{
	// Full screen popup window for our game
	rWindow d;
		d.clientW = Sys::GetScreenSize().x; //FULL SCREEN props
		d.clientH = Sys::GetScreenSize().y; //FULL SCREEN props
		d.style = eWindowStyle::FULLSCREEN; //FULL SCREEN props
	IWindow* window = new Window(d);

	// Hide hardware cursor for our game on window
	window->SetCursorVisible(false);

	// Init Engine core
	Core::Instantiate();
	rGraphicsEngineRaster graphicsDesc;
		graphicsDesc.gapiType = eGapiType::OPENGL_4_5;
		graphicsDesc.targetWindow = window;
	gCore->InitGraphicsEngineRaster(graphicsDesc);
	gCore->InitPhysicsEngineBullet();
	gCore->InitSoundEngineSFML();
	
	ITimer* timer = new Timer();
	timer->Start();

	InitScript(); // Manual bullshit, TODO !!!

	while (window->IsOpen())
	{
		// Prepare for input processing
		gInput.ClearFrameData();

		// Process input events coming from O.S.-> Window
		rWindowEvent evt;
		while(window->PopEvent(evt))
		{
			switch(evt.msg)
			{
			case eWindowMsg::KEY_PRESS:
				gInput.KeyPress(evt.key);
				break;

			case eWindowMsg::KEY_RELEASE:
				gInput.KeyRelease(evt.key);
				break;

			case eWindowMsg::MOUSE_MOVE:
			{
				assert(evt.x >= 0 && evt.y >= 0);
				gInput.MouseMove(mm::ivec2(evt.deltaX, evt.deltaY), mm::uvec2((u32)evt.x, (u32)evt.y));
				break;
			}

			case eWindowMsg::MOUSE_PRESS:
				switch (evt.mouseBtn)
				{
				case eMouseBtn::LEFT:  gInput.MouseLeftPress();  break;
				case eMouseBtn::MID:   gInput.MouseMidPress();	 break;
				case eMouseBtn::RIGHT: gInput.MouseRightPress(); break;
				}
				break;

			case eWindowMsg::MOUSE_RELEASE:
				switch (evt.mouseBtn)
				{
				case eMouseBtn::LEFT:  gInput.MouseLeftRelease();  break;
				case eMouseBtn::MID:   gInput.MouseMidRelease();   break;
				case eMouseBtn::RIGHT: gInput.MouseRightRelease(); break;
				}
				break;
			}
		}

		gInput.Update();

		float deltaSeconds = timer->GetSecondsPassed();
		timer->Reset();

		gCore->Update(deltaSeconds);
	}
	return 0;
}

void InitScript()
{
	gCore->AddScript<TestLevelScript>();
}