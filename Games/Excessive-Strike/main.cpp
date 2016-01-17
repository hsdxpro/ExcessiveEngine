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

void InitScript();

int main()
{
	// Full screen popup window for our game
	rWindow d;
		d.clientSize = mm::uvec2(1024, 768);
		//d.bVSync = true;
	Window* window = new Window(d);

	// Init Engine core
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
				//assert(evt.x >= 0 && evt.y >= 0);
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

	delete window;
	delete timer;
	return 0;
}

void InitScript()
{
	World.AddScript<TestLevelScript>();
}