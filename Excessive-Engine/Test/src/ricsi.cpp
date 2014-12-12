#include "tests.h"
#include "Factory.h"
#include "IWindow.h"
#include "GraphicsEngine"
#include "..\GraphicsApi_Interface\interface\Common.h"
#include "..\GraphicsApi_Interface\interface\IShaderProgram.h"
#include "..\GraphicsApi_Interface\interface\IGapi.h"

#include <conio.h>

#include <chrono>
#include <thread>

using namespace std;


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
IGapi*					gGapi;
IWindow*				gWindow;

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
	rShaderSources s;
		s.vsSrc = vsSimple;
		s.psSrc = psSimple;
	gGapi->createShaderProgram(s);

	// Create texture that we draw as FSQ
	//ITexture* tex = gGapi->createTexture(Sys::getWorkDir() + "image.png");

	// Create vertexbuffer Pos texcoord

	// Create indexbuffer

	// Create uniforms (ortographic) camera


	// Create some graphics engine entities
	graphics::IScene* scene = gEngine->createScene();
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
	uint32_t indices[] = {
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
	data.index_num = sizeof(indices) / sizeof(uint32_t);
	data.mat_ids = nullptr;
	data.mat_ids_num = 0;

	bool is_mesh = mesh->update(data);

	if (!is_mesh) {
		cout << "Could not create a mesh :(" << endl;
	}
	else {
		cout << "Mesh created!" << endl;
	}

	// Assign mesh to entity
	entity->setMesh(mesh);

	// So that I can read init messages
	this_thread::sleep_for(chrono::milliseconds(1000));


	// Run the main loop
	rWindowEvent ev;
	double elapsed;
	chrono::time_point<chrono::high_resolution_clock> last_frame;

	while (gWindow->isOpen()) {
		cout << "----------------------------------------------------" << endl;
		// keep 60 fps
		elapsed = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - last_frame).count() / 1.0e6;
		last_frame = chrono::high_resolution_clock::now();

		while (gWindow->popEvent(&ev))
			if (ev.msg == eWindowMsg::KEY_PRESS && ev.key == eKey::ESCAPE)
				gWindow->close();

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