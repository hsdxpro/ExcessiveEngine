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


#include "Core/EngineCore.h"
#include "Core/CameraComponent.h"
#include "Core/InputCore.h"
#include "Core/EngineCore.h"
#include "PlatformLibrary/Window.h"
#include "GraphicsEngine/RasterZsiros\Core\src\zsString.h"

#include "PlayerScript.h"
#include "TestLevelScript.h"

#include "PlatformLibrary/Timer.h"
#include "PlatformLibrary/File.h"
#include "SupportLibrary/VisualCpuProfiler.h"

#include <string>

Window* window;
ISoundEngine* soundEngine;
IGuiEngine* guiEngine;
IGraphicsEngine* graphicsEngine;
IPhysicsEngine* physicsEngine;

void InitGameScripts();
void InitGui();

void main()
{
	// Create Game Window
	WindowDesc d;
	//d.clientSize = mm::uvec2(Sys::GetScreenSize().x, Sys::GetScreenSize().y);
	d.clientSize = mm::uvec2(800, 600);
	d.style = eWindowStyle::BORDERLESS;
	window = new Window(d);

	// Init Sound Engine
	soundEngine = Core.InitSoundEngineSFML();

	// Init Graphics Engine
	GraphicsEngineRasterDesc graphicsDesc;
	graphicsDesc.gapiType = eGapiType::DX11;
	graphicsDesc.targetWindow = window;
	graphicsEngine = Core.InitGraphicsEngineRasterZsiros(graphicsDesc);

	// Init Physics Engine
	PhysicsEngineBulletDesc physicsDesc;
	physicsDesc.gravity = mm::vec3(0, -9.81f, 0);
	physicsEngine = Core.InitPhysicsEngineBullet(physicsDesc);

	// Init Gui Engine
	guiEngine = Core.InitGuiEngine();

	// Init gui
	InitGui();

	// Init Game Scripts
	InitGameScripts(); // Manual bullshit, TODO !!!

	// Create timer, delta time -> engine
	Timer* timer = new Timer();
	timer->Start();


	// Main loop, till window open
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
				} break;

				case KEY_RELEASE:
				{
					if (evt.key != INVALID_eKey)
						Input.KeyRelease(evt.key);
				} break;

				case MOUSE_MOVE:
				{
					Input.MouseMove(mm::ivec2(evt.deltaX, evt.deltaY), mm::ivec2(evt.x, evt.y));
				} break;

				case MOUSE_PRESS:
				{
					switch (evt.mouseBtn)
					{
						case LEFT:	Input.MouseLeftPress();		break;
						case RIGHT:	Input.MouseRightPress();	break;
						case MID:	Input.MouseMidPress();		break;
					} 
				} break;

				case MOUSE_RELEASE:
				{
					switch (evt.mouseBtn)
					{
						case LEFT:	Input.MouseLeftRelease();	break;
						case RIGHT: Input.MouseRightRelease();	break;
						case MID:	Input.MouseMidRelease();	break;
					} 
				} break;
			}
		}

		// Dispatch Inputs
		Input.Update();

		// IsKeyPressed Enterre sose lesz igaz, mert asszem hogy a window message - ben kétszer szerepel az enter megnyomása, mert system message is jön, meg egy nem system message is
		if(Input.IsKeyPressed(ENTER))
		{
			u16 width = window->GetClientWidth();
			u16 height = window->GetClientHeight();

			graphicsEngine->ResizeRenderTargets(width, height);
		}

		// Get delta seconds from the timer
		float deltaSeconds = timer->GetSecondsPassed();
		timer->Reset();

		// Go Excessive Engine go !!
		Core.Update(deltaSeconds);
	}

	// Cleanup
	delete window;
	delete timer;
}

void InitGameScripts()
{
	World.AddScript<TestLevelScript>();
}

void InitGui()
{
	// Canvas and Layer
	GuiCanvas* canvas = guiEngine->AddCanvas();
	GuiLayer* layer = canvas->AddLayer();

	// Test button
	GuiButton* button = layer->AddButton();
	button->SetBackgroundToColor(Color::RED);
	button->SetRect(0, 0, 100, 50);
	//button->SetText("Button");
}