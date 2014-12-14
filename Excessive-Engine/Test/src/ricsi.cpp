#include "tests.h"
#include "Factory.h"
#include "IWindow.h"
#include "GraphicsEngine"
#include "..\GraphicsApi_Interface\interface\Common.h"
#include "..\GraphicsApi_Interface\interface\IShaderProgram.h"
#include "..\GraphicsApi_Interface\interface\IGapi.h"

#include <conio.h>
#include <string>
#include <chrono>
#include <thread>
#include "ICamera.h"
#include "ICamera.h"

using namespace std;

#define CAM_MOVE_SPEED 10
float gCamSpeedMultiplier = 1;

const char* vsSimple =
"#version 440 core \n"
"layout(std140) uniform constant_data \n"
"{ \n"
"  mat4 mvp; \n"
"} cd; \n"
"in vec3 in_vertex; \n"
"in vec2 in_texcoord; \n"
"out vec2 texcoord; \n"
"void main() \n"
"{ \n"
"  texcoord = in_texcoord; \n"
"  gl_Position = cd.mvp * vec4(in_vertex, 1); \n"
"} \n";

const char* psSimple =
"#version 440 core \n"
"uniform sampler2D tex; \n"
"in vec2 texcoord; \n"
"out vec4 color; \n"
"void main() \n"
"{ \n"
"  color = texture(tex, vec2(texcoord.x, 1-texcoord.y)); \n"
"} \n";


graphics::IGraphicsEngine*	gEngine;
graphics::ICamera*			gCam;
IGapi*						gGapi;
IWindow*					gWindow;


int Ricsi() {

	// Init window
	rWindow d;
	d.clientW = 800;
	d.clientH = 600;
	d.capText = "Excessive-Engine -> Ricsi teszt";
	gWindow = Factory::createWindow(d);

	// Init GraphicsEngine
	graphics::rGraphicsEngine gDesc;
		gDesc.gapi = Factory::createGapiGL();
	gEngine = Factory::createGraphicsEngineRaster(gDesc);
	if (!gEngine) {
		cout << "Oops, failed to create graphics engine :(" << endl;
		cout << "Press any key to exit..." << endl;
		_getch();
		return 0;
	}

	// Get the API belonging to graphics engine
	gGapi = gEngine->getGapi();

	// Create simple shaderProgram
	auto shader = gGapi->createShaderSource(vsSimple, psSimple);

	// Create texture that we draw as FSQ
	//ITexture* tex = gGapi->createTexture(Sys::getWorkDir() + "image.png");

	// Create vertexbuffer Pos texcoord

	// Create indexbuffer

	// Create uniforms (ortographic) camera


	// Create some graphics engine entities
	graphics::IScene* scene = gEngine->createScene();
	// Tápoljunk valami kamerát a scene - hez

	gCam = gEngine->createCam();
		gCam->setFOV(3.14 / 2);
		gCam->setAspectRatio(gWindow->getClientAspectRatio());
		gCam->setNearPlane(0.05);
		gCam->setFarPlane(3000);
		gCam->setPos(mm::vec3(0, -3, 1));
	scene->setCam(gCam);

	graphics::IEntity* entity = scene->createEntity();
	
	// Create a simple whatever
	graphics::IMesh* mesh = gEngine->createMesh();
	graphics::IMesh::MeshData data;
	//   [0]---[1]
	//   /  \  /
	//  [3]---[2]
	//
	//   [4]---[5]
	//   /  \  /
	//  [7]---[6]
	float vertices[] = {
		-1, 1, 1,
		1, 1, 1,
		1, -1, 1,
		-1, -1, 1,
		-1, 1, -1,
		1, 1, -1,
		1, -1, -1,
		-1, -1, -1,
	};
	u32 indices[] = {
		1,0,2, // top
		2,3,0,
		4,5,6, // bottom
		7,4,6,
		2,3,6, // front
		3,7,6,
		0,1,5, // back
		5,4,0,
		1,2,6, //right
		1,6,5,
		7,3,0, // left
		4,7,0,
	};
	graphics::IMesh::ElementDesc elements[] = {
		graphics::IMesh::POSITION, 3,
	};
	data.vertex_data = vertices;
	data.vertex_bytes = sizeof(vertices);
	data.vertex_elements = elements;
	data.vertex_elements_num = 1;
	data.index_data = indices;
	data.index_num = sizeof(indices) / sizeof(u32);
	data.mat_ids = nullptr;
	data.mat_ids_num = 0;

	bool is_mesh = mesh->update(data);

	if (!is_mesh) {
		cout << "Could not create a mesh :(" << endl;
	}
	else {
		cout << "Mesh created!" << endl;
	}


	
	Factory::createResourceLoader()->loadMesh(mesh, (Sys::getWorkDir() + std::wstring(L"teapot.dae")).c_str());

	// Assign mesh to entity
	entity->setMesh(mesh);

	// So that I can read init messages
	this_thread::sleep_for(chrono::milliseconds(1000));


	// Run the main loop
	rWindowEvent ev;
	double elapsed;
	chrono::time_point<chrono::high_resolution_clock> last_frame;

	// Timer for dT frame calc
	//ITimer* t = Factory::createTimer();

	bool bWDown = false;
	bool bSDown = false;
	bool bADown = false;
	bool bDDown = false;
	
	bool isOpen = true;
	bool bRMBDown = false;

	while (isOpen) {
		//t->reset();
		isOpen = gWindow->isOpen();
		//cout << "----------------------------------------------------" << endl;
		// keep 60 fps
		elapsed = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - last_frame).count() / 1.0e6;
		last_frame = chrono::high_resolution_clock::now();

		while (gWindow->popEvent(&ev))
			switch (ev.msg)
			{
			case eWindowMsg::MOUSE_PRESS:
				if (ev.mouseBtn == eMouseBtn::RIGHT)
					bRMBDown = true;
				break;
			case eWindowMsg::MOUSE_RELEASE:
				if (ev.mouseBtn == eMouseBtn::RIGHT)
					bRMBDown = false;
				break;
			case eWindowMsg::MOUSE_MOVE: {
				 if (bRMBDown)
				 {
					// TODO *= ne ezt csinalja mar
					/*
					mm::vec3 v1 = (0, 1, 0);
					mm::quat rot(3.1415 / 2, mm::vec3(0, 0, 1));
					v1 *= rot;
					*/

					//mm::vec3 rotedVec = gCam->getDirFront();
					//
					//mm::quat rotAroundZ((float)ev.mouseDx / 1000, mm::vec3(0,0,1));
					////mm::quat rotAroundX((float)ev.mouseDy / 1000, mm::vec3(1, 0, 0));
					//
					//rotedVec = mm::rotate_vector(rotAroundZ, rotedVec);
					////rotedVec = mm::rotate_vector(rotAroundX, rotedVec);
					//
					//gCam->setTarget(gCam->getPos() + rotedVec);
				 }
			} break;

			case eWindowMsg::KEY_PRESS:
				switch (ev.key)
				{
				case eKey::ESCAPE: gWindow->close(); break;
				case eKey::W: bWDown = true; break;
				case eKey::S: bSDown = true; break;
				case eKey::A: bADown = true; break;
				case eKey::D: bDDown = true; break;
				case eKey::LSHIFT: gCamSpeedMultiplier = 5; break;
				} break;
			case eWindowMsg::KEY_RELEASE:
				switch (ev.key)
				{
				case eKey::W: bWDown = false; break;
				case eKey::S: bSDown = false; break;
				case eKey::A: bADown = false; break;
				case eKey::D: bDDown = false; break;
				case eKey::LSHIFT: gCamSpeedMultiplier = 1; break;
				} break;
			}

		// Camera move
		if (bWDown) // W
			gCam->setPos(gCam->getPos() + gCam->getDirFront() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
		if (bSDown) // S
			gCam->setPos(gCam->getPos() + gCam->getDirBack()  *	CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
		if (bADown) // A
			gCam->setPos(gCam->getPos() + gCam->getDirLeft()  *	CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
		if (bDDown) // D
			gCam->setPos(gCam->getPos() + gCam->getDirRight() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
			
		// Update everything
		
		//float deltaT = t->getElapsedSinceReset();

		// CLear frame buffer
		gGapi->clearFrameBuffer(eClearFlag::COLOR_DEPTH, mm::vec4(0, 0, 0, 0), 0, 0);

		// Update graphics engine
		gEngine->update();

		// Call that after OpenGL "finish" all of it's rendering
		gWindow->displayClientRect();

		// keep 60 fps
		chrono::time_point<chrono::high_resolution_clock> now = chrono::high_resolution_clock::now();
		chrono::microseconds sleep_time = chrono::microseconds(16667) - chrono::duration_cast<chrono::microseconds>(now - last_frame);
		this_thread::sleep_for(sleep_time);
	}
	cout << endl;
	
	return 0;
}