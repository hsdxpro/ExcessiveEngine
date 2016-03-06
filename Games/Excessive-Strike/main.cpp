// GuiSystem
// Canvas
// - GuiPages (DEPRECATED)
//		- GuiLayers
//			- GuiControls

// Gui library használata:

// GuiSystem
// GuiSystemPeter

//* GuiSytemRichard::CreateButton(.....)

// Typical Usage
// GuiSystemRichard guiSys;
// GuiSystemRobert guiSys;
// guiSys.CreateLayer(...) CreateButton(...) // MyButtonImpl* button = CreateButton(...)
// OffScreenBuffer* buf = GuiRendererCpu.Render(GuiSystemRichard);
// buf->bitBltToScreen(...);

// GuiRendererDx11	 : IGuiRenderer // Dx11 resource - ba belerenderel mindent
// GuiRendererOpenGL : IGuiRenderer // OpenGL resource - ba belerenderel mindent
// GuiRendererCpu	 : IGuiRenderer // byte* ptr - be belerenderel mindent


#include "Core\EngineCore.h"
#include "Core\CameraComponent.h"
#include "PlatformLibrary\Window.h"
#include "PlatformLibrary\Timer.h"
#include "SupportLibrary\VisualCpuProfiler.h"
#include "PlayerScript.h"
#include "TestLevelScript.h"
#include "Core\InputCore.h"
#include "PlatformLibrary\File.h"
#include "Core\EngineCore.h"
#include <string>
#include "GraphicsEngine_RasterZsiros\Core\src\zsString.h"

void InitScript();

int main()
{
	// Full screen popup window for our game
	rWindow d;
		d.clientSize = mm::uvec2(800, 600);
		//d.bVSync = true;
	Window* window = new Window(d);

	// Init Engine core
	Core.InitSoundEngineSFML();
	rGraphicsEngineRaster graphicsDesc;
		graphicsDesc.gapiType = eGapiType::DX11;
		graphicsDesc.targetWindow = window;
	Core.InitGraphicsEngineRasterZsiros(graphicsDesc);
	rPhysicsEngineBullet physicsDesc;
		physicsDesc.gravity = mm::vec3(0, 0, -9.81);
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
			case KEY_PRESS:
			{
				if (evt.key != INVALID_eKey)
					Input.KeyPress(evt.key);

				if (evt.key == ENTER)
					OutputDebugStringW(L"ENTER_PRESS\n");

				break;
			}

			case KEY_RELEASE:
			{
				if (evt.key != INVALID_eKey)
					Input.KeyRelease(evt.key);

				if (evt.key == ENTER)
					OutputDebugStringW(L"ENTER_RELEASE\n");

				break;
			}

			case MOUSE_MOVE:
			{
				Input.MouseMove(mm::ivec2(evt.deltaX, evt.deltaY), mm::ivec2(evt.x, evt.y));
				break;
			}

			case MOUSE_PRESS:
				switch (evt.mouseBtn)
				{
				case LEFT:	Input.MouseLeftPress();		break;
				case RIGHT:	Input.MouseRightPress();	break;
				case MID:	Input.MouseMidPress();		break;
				}
				break;

			case MOUSE_RELEASE:
				switch (evt.mouseBtn)
				{
				case LEFT:	Input.MouseLeftRelease();	break;
				case RIGHT: Input.MouseRightRelease();	break;
				case MID:	Input.MouseLeftRelease();	break;
				}
				break;
			}
		}

		Input.Update();

		// IsKeyPressed Enterre sose lesz igaz, mert asszem hogy a window message - ben kétszer szerepel az enter megnyomása, mert system message is jön, meg egy nem system message is
		if(Input.IsKeyPressed(ENTER))
		{
			OutputDebugStringW(L"ALT + ENTER\n");
			//window->Close();
			//break;
		}

		if (Input.IsKeyReleased(ENTER))
		{
			OutputDebugStringW(L"e r\n");
			//window->Close();
			//break;
		}

		if (Input.IsKeyDown(ENTER))
		{
			//OutputDebugStringW(L"Enter pressed\n");
		}

		float deltaSeconds = timer->GetSecondsPassed();
		timer->Reset();

		Core.Update(deltaSeconds);
	}

	delete window;
	delete timer;
	return 0;
}

void InitScript()
{
	World.AddScript<TestLevelScript>();
}