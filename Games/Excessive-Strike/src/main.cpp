#include "..\Common\src\Factory.h"
#include "..\Common\src\IWindow.h"
#include "..\Common\src\Sys.h"
#include "..\Common\src\EngineCore.h"
#include "..\Common\src\ITimer.h"

// Spirit of the engine
EngineCore gEngineCore;

// Window for our game
IWindow* gWindow;

ComponentCamera* gCamComp;

int main()
{
	// Full screen popup window for our game
	rWindow d;
		//d.style = eWindowStyle::FULLSCREEN;
		d.style = eWindowStyle::TITLE__RESIZE__CLOSE;
		d.clientW = 800;// Sys::getScreenSize().x;
		d.clientH = 600;// Sys::getScreenSize().y;
	gWindow = Factory::createWindow(d);

	// Init Engine core
	rGraphicsEngineRaster graphicsDesc;
		graphicsDesc.gapiType = eGapiType::OPENGL_4_5;
		graphicsDesc.targetWindow = gWindow;
	gEngineCore.initGraphicsEngineRaster(graphicsDesc);
	gEngineCore.initPhysicsEngineBullet();
	gEngineCore.initSoundEngine();

	ITimer* timer = Factory::createTimer();
	timer->start();

	// Create camera
	//auto cam = gEngineCore.getGraphicsEngine()->createCam();
	//	cam->setPos(mm::vec3(0, -3, 10));
	//gEngineCore.getDefaultGraphicsScene()->setCamera(cam);

	// Add some actors
	auto& groundModelPath = Sys::getWorkDir() + L"../Assets/demo_ground.dae";
	auto& skyModelPath = Sys::getWorkDir() + L"../Assets/skybox.dae";
	auto& ak47ModelPath = Sys::getWorkDir() + L"../Assets/ak47/ak.obj";
	auto& boxModelPath = Sys::getWorkDir() + L"../Assets/box.dae";

	// Terrain & Sky
	gEngineCore.addCompRigidBodyFromFile(groundModelPath, 0)->attach(gEngineCore.addCompGraphicsFromFile(groundModelPath));
	gEngineCore.addCompGraphicsFromFile(skyModelPath)->setScale({ 1000, 1000, 1000 });
	//
	// Player components
	//auto compPhysics = gEngineCore.addCompRigidBodyCapsule(2, 0.2, 20);
	gCamComp = gEngineCore.addCompCamera();
	//auto compGraphics	= gEngineCore.addCompGraphicsFromFile(ak47ModelPath);
	//	compGraphics->setScale({ 1.f / 500, 1.f / 500, 1.f / 500 });
	//	//compGraphics->setRot(mm::quat(-0.05, {1,0,0}) * mm::quat(3.1415 - 0.08, { 0, 0, 1 }) * mm::quat(3.1415 / 2, { 1, 0, 0 })); // Need quaternion rework, multiply swapped, mm::quat(angle) rots with -angle, why?
	////
	////// Build Player component tree
	//	gCamComp->setPos({ 0.f, 0.f, 1.8f });
	//	//compGraphics->setPos(gCamComp->getPos() + mm::vec3(1.4, 2, -2.8));
	//
	////compPhysics->addChild(compCamera);
	//gCamComp->setPos({ 0, -8, 4 });
	//
	//compPhysics->attach(compGraphics);
	//compPhysics->attach(gCamComp);
	//
	//
	//compPhysics->setPos({ 0.f, 0.f, 10.f });
	//
	gEngineCore.setCam(gCamComp);

	//auto mat = gEngineCore.createGraphicsMaterial();
	//graphics::IMaterial::SubMaterial s;
	//	s.t_diffuse = gEngineCore.createGraphicsTexture();
	//	s.t_diffuse->load(Sys::getWorkDir() + )
	//mat->addSubMaterial()
	//comp->setMaterial(mat);

	while (gWindow->isOpen())
	{
		rWindowEvent evt;
		while (gWindow->popEvent(&evt))
		{
			if (evt.msg == eWindowMsg::MOUSE_RELEASE && evt.mouseBtn == eMouseBtn::LEFT)
			{

				//auto c0 = gEngineCore.addCompGraphicsFromFile(boxModelPath);
				//auto c1 = gEngineCore.addCompGraphicsFromFile(boxModelPath);
				//
				//gCamComp->setPos({0,-12, 5});
				//
				//c0->setScale({ 1.f / 20, 1.f / 20, 1.f / 20 });
				//c1->setScale({ 1.f / 20, 1.f / 20, 1.f / 20 });
				//
				//
				//c0->setPos({ 1, 0, 4 });
				//c1->setPos({ -1, 0, 2 });
				//
				//c0->attachChild(c1);
				//
				//c0->setScale(c1->getScale() * 2);
				//c0->setRot(mm::quat(3.14159265 / 4, { 0, 0, 1 }));



				auto compPhysics = gEngineCore.addCompRigidBodyFromFile(boxModelPath, 20);
				compPhysics->setScale({ 1.f / 20, 1.f / 20, 1.f / 20 });
				compPhysics->setPos({ 0, 0, 30.f });

				WorldComponent* boxes[1];
				for (size_t i = 0; i < 1; i++)
				{
					boxes[i] = gEngineCore.addCompGraphicsFromFile(boxModelPath);
					boxes[i]->setScale({ 1.f / 20, 1.f / 20, 1.f / 20 });
				}

				mm::vec3 pos = compPhysics->getPos();
				const float dist = 2;
				boxes[0]->setPos(pos + mm::vec3(0, 0, 0));
				//boxes[1]->setPos(pos + mm::vec3(0, 0, dist));
				//boxes[2]->setPos(pos + mm::vec3(0, 0, -dist));
				//boxes[3]->setPos(pos + mm::vec3(0, dist, 0));
				//boxes[4]->setPos(pos + mm::vec3(0, -dist, 0));
				//boxes[5]->setPos(pos + mm::vec3(dist, 0, 0));
				//boxes[6]->setPos(pos + mm::vec3(-dist, 0, 0));

				compPhysics->attach(boxes[0]);
				//compPhysics->attach(boxes[1]);
				//compPhysics->attach(boxes[2]);
				//compPhysics->attach(boxes[3]);
				//compPhysics->attach(boxes[4]);
				//compPhysics->attach(boxes[5]);
				//compPhysics->attach(boxes[6]);

				//gCamComp->detach();
				//gCamComp->detach();
				gCamComp->detach();
				gCamComp->setPos(pos + mm::vec3(0, -13, 0));

				//mm::quat asd = compPhysics->getRot();
				//gCamComp->setRot(asd);

				mm::vec3 dir = boxes[0]->getPos() - gCamComp->getPos();
					//dir.z = abs(dir.z + 0.2);
				gCamComp->setDirNormed(mm::normalize(dir));

				compPhysics->attach(gCamComp);
			}
		}

		float deltaSeconds = timer->getSecondsPassed();
		timer->reset();

		gEngineCore.update(deltaSeconds);
		gWindow->present();
	}
	return 0;
}