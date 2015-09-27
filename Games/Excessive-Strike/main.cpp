// Mai feladat : Gondolkodni kene Core feldarabolasan, Physics, Sound, Graphics, Network
// A felosztás mûködõképes ha individuális modul részeket ölelnek fel a felosztott részek
// De mi van olyan funckionalitással amihez pl graphicsEngine meg phyicsEngine is kell, ez egyértelmûen a Core dolga
// Nézzük meg, hogy néznének ki ezek az osztályok

// Input
// Network
// Graphics
// Physics
// Sound
// World
// Core Amikor Core példányosul, akkor a többi is
// Core példányosítását nem vethetjük el editoron belül, de játékon belül lesz egy Core

// Egyszerû eset:
// Akarok egy GuiImage : GuiControl a képernyõ közepére amin megjelenik az alma.jpg

// Editorbol ?? "New Gui Page(mainMenu)", lepakolom a control - t, majd dragelek alma.jpg oda
// Kódból beállítom a GuiLayer - t mainMenu - re
// Canvas
// - GuiPages
//		- GuiLayers
//			- GuiControls

// A GuiLibrary felelössége
// - Megjelenítéshez szükséges absztrakció hordozása
// Vannak layerek, pl inventory, hp and mana
// Van layer, pl MainMenu
// Layerek sokaságát GuiPage - nek hívjuk
// Minden stackelhetõ, <GuiPage>, <GuiLayer>, <GuiControl>

// Ne bonyolítsuk túl. Pl van egy inventory nyitva level 0 - nál
// level1 - re pusholok egy teljes mainMenu - t

// Másik eset, az editoron belül bizonyos viewport részre akarom renderelni az egész világot
// gui lib renderelés lefut aztán csókolom, ezután Core - n keresztül beállítom hogy az ablak mely részére kéne renderelni a GuiControl alapján, majd meghívom az update - t
// Ezt mind az editor mainLoop - jában

// Core eddig hangot, .dae stb fájlokat tud zúzni

#include "Core\EngineCore.h"
#include "Core\CameraComponent.h"
#include "PlatformLibrary\Window.h"
#include "PlatformLibrary\Timer.h"
#include "SupportLibrary\VisualCpuProfiler.h"
#include "PlayerScript.h"
#include "TestLevelScript.h"
#include "Core\InputCore.h"
#include "PlatformLibrary\File.h"
#include "Core\WorldCore.h"

void InitScript();

int main()
{
	// Full screen popup window for our game
	rWindow d;
		d.clientW = Sys::GetScreenSize().x; //FULL SCREEN props
		d.clientH = Sys::GetScreenSize().y; //FULL SCREEN props
		d.style = eWindowStyle::FULLSCREEN; //FULL SCREEN props
	Window* window = new Window(d);

	// Hide hardware cursor for our game on window
	window->SetCursorVisible(false);

	// Init Engine core
	//EngineCore::InstantiateSingleton();
	Core.InitSoundEngineSFML();
	rGraphicsEngineRaster graphicsDesc;
		graphicsDesc.gapiType = eGapiType::OPENGL_4_5;
		graphicsDesc.targetWindow = window;
	Core.InitGraphicsEngineRaster(graphicsDesc);
	rPhysicsEngineBullet physicsDesc;
		physicsDesc.gravity = mm::vec3(0, 0, -9.81f);
	Core.InitPhysicsEngineBullet(physicsDesc);	

	InitScript(); // Manual bullshit, TODO !!!

	Timer* timer = new Timer();
	timer->Start();

	while (window->IsOpen())
	{
		// Prepare for input processing
		Input.ClearFrameData();

		// Process input events coming from O.S.-> Window
		rWindowEvent evt;
		while(window->PopEvent(evt))
		{
			switch(evt.msg)
			{
			case eWindowMsg::KEY_PRESS:
				if (evt.key != eKey::INVALID)
					Input.KeyPress(evt.key);
				break;

			case eWindowMsg::KEY_RELEASE:
				if (evt.key != eKey::INVALID)
					Input.KeyRelease(evt.key);
				break;

			case eWindowMsg::MOUSE_MOVE:
			{
				assert(evt.x >= 0 && evt.y >= 0);
				Input.MouseMove(mm::ivec2(evt.deltaX, evt.deltaY), mm::uvec2((u32)evt.x, (u32)evt.y));
				break;
			}

			case eWindowMsg::MOUSE_PRESS:
				switch (evt.mouseBtn)
				{
				case eMouseBtn::LEFT:  Input.MouseLeftPress();  break;
				case eMouseBtn::MID:   Input.MouseMidPress();	 break;
				case eMouseBtn::RIGHT: Input.MouseRightPress(); break;
				}
				break;

			case eWindowMsg::MOUSE_RELEASE:
				switch (evt.mouseBtn)
				{
				case eMouseBtn::LEFT:  Input.MouseLeftRelease();  break;
				case eMouseBtn::MID:   Input.MouseMidRelease();   break;
				case eMouseBtn::RIGHT: Input.MouseRightRelease(); break;
				}
				break;
			}
		}

		Input.Update();

		float deltaSeconds = timer->GetSecondsPassed();
		timer->Reset();

		Core.Update(deltaSeconds);
	}

	return 0;
}

void InitScript()
{
	World.AddScript<TestLevelScript>();
}