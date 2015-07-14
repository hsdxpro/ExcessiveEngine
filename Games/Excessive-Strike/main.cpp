// Nagyon fontos !! 
// Ha Behavior mutál Script nem feltétlen !!
// Ha Script mutál, Behavior nem feltétlen


// Csinálj egy olyan unit test - et, hogy az alábbit kényelmesen meglehessen csinálni !
// Adott Thing - ek
// Thing0 Thing1 Thing2 Thing3 Thing4
// Actor -> gömb grafika
// Behavior -> mozgás balra jobbra

// Gomb nyomásra Thing1 és Thing2 behaviorja alá tartozó scripteket módosítani akarom hogy nagyobb mozgást végezzen, de ezt csak Thing1 és Thing2 - re
// Szóval azt csinálom hogy másolok egy új behaviort beállítom mindkettõnek, majd a behavior script - jét módosítom így Thing1 és Thing2 gyorsabban fog mozogni
//


// Elmélet
//// Mibõl elhet Thing - et létrehozni?
//// ActorBehavior, Actor, WorldComponent, ActorScript  // ezek közül csak ActorScript lehet templateType
//
//// Editor oldalon összepakolok egy Thing - et, amiben van csomó komponens, és ráaggatok néhány script - et
//// Thing* myType ....
//
//Actor* constructedActor = gCore.AddActor();
//// Sok komponens hozzá csatolása....
//ActorBehavior* behavior = gCore.AddBehavior();
//// Script beállítása
//
//// Elkezdõdik a level ->
//Thing* thingType = gCore.CreateThing();
//thingType->SetActor(constructedActor);
//thingType->AddBehavior(behavior);
//
//
//// Ide kell pár flag hogy bDeepCopyActor, bDeepCopyBehavior, bDeepCopyScript
//// bUniqScript, bUniqActor, bUniqBehavior
//auto thing0 = gCore.SpawnThing(thingType, eCopy::ACTOR); // És emiatt belül a thingType->actor - ból deepCopy lesz az új thing - ben, nem csak pointer csere
//auto thing1 = gCore.SpawnThing(thingType, eCopy::ACTOR);
//auto thing2 = gCore.SpawnThing(thingType, eCopy::ACTOR);
//
//thing0->setPos({ 0, 0, 0 });
//thing1->setPos({ 10, 10, 10 }); // Ez fogja és közvetlen az actor RootComponent -jének a pozícióját átállítja
//// Szóval amikor Thing - et spawnolok thing - bõl, akkor az Actor - t úgy ahogy van deep copyzom, de a Behavior - t azt nem


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
	window->HideCursor();

	// Init Engine core
	rGraphicsEngineRaster graphicsDesc;
		graphicsDesc.gapiType = eGapiType::OPENGL_4_5;
		graphicsDesc.targetWindow = window;
	gCore.InitGraphicsEngineRaster(graphicsDesc);
	gCore.InitPhysicsEngineBullet();
	gCore.InitSoundEngineSFML();
	
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

		gCore.Update(deltaSeconds);
	}
	return 0;
}

void InitScript()
{
	gCore.AddScript<TestLevelScript>();
}