// GuiSystem
// Canvas
//	- GuiLayers
//		- GuiControls

// Gui library használata:

// GuiSystem
// GuiSystemPeter

//* GuiSytemRichard::CreateButton(.....)

// Typical Usage
// GuiSystemRichard guiSys;
// GuiSystemRobert guiSys;
// guiSys.Addayer(...) AddButton(...) // MyButtonImpl* button = AddButton(...)

// Render
// GuiRendererCpu.Render(GuiSystemRichard, );
//NOPE // buf->bitBltToScreen(...);


// GuiRendererDx11	 : IGuiRenderer // Dx11 resource - ba belerenderel mindent
// GuiRendererOpenGL : IGuiRenderer // OpenGL resource - ba belerenderel mindent
//NOPE // GuiRendererCpu	 : IGuiRenderer // byte* ptr - be belerenderel mindent


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
	WindowDesc d;
		//d.clientSize = mm::uvec2(Sys::GetScreenSize().x, Sys::GetScreenSize().y);
		d.clientSize = mm::uvec2(800, 600);
		d.style = eWindowStyle::BORDERLESS;
	Window* window = new Window(d);

	// Init Engine core
	Core.InitSoundEngineSFML();
	GraphicsEngineRasterDesc graphicsDesc;
		graphicsDesc.gapiType = eGapiType::DX11;
		graphicsDesc.targetWindow = window;

	IGraphicsEngine* graphicsEngine = Core.InitGraphicsEngineRasterZsiros(graphicsDesc);

	PhysicsEngineBulletDesc physicsDesc;
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
		WindowEvent evt;
		while(window->PopEvent(evt))
		{
			switch(evt.msg)
			{
			case KEY_PRESS:
			{
				if (evt.key != INVALID_eKey)
					Input.KeyPress(evt.key);
				break;
			}

			case KEY_RELEASE:
			{
				if (evt.key != INVALID_eKey)
					Input.KeyRelease(evt.key);
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
			u16 width = window->GetClientWidth();
			u16 height = window->GetClientHeight();

			graphicsEngine->ResizeRenderTargets(width, height);
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