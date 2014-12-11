#include "tests.h"
#include "Factory.h"
#include "IWindow.h"
#include "GraphicsEngine"
#include "..\GraphicsApi_Interface\interface\Common.h"
#include "..\GraphicsApi_Interface\interface\IShaderProgram.h"
#include "..\GraphicsApi_Interface\interface\IGapi.h"

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

	// Get the API belonging to graphics engine
	gGapi = gEngine->getGapi();

	// Create simple shaderProgram
	rShaderSources s;
		s.vsSrc = vsSimple;
		s.psSrc = psSimple;
	gGapi->createShaderProgram(s);

	// Create texture that we draw as FSQ
	ITexture* tex = gGapi->createTexture(Sys::getWorkDir() + "image.png");

	// Create vertexbuffer Pos texcoord

	// Create indexbuffer

	// Create uniforms (ortographic) camera


	// Create some graphics engine entities
	graphics::IScene* scene = gEngine->createScene();
	graphics::IEntity* entity = scene->createEntity();


	rWindowEvent ev;
	double elapsed;
	chrono::time_point<chrono::high_resolution_clock> last_frame;

	while (gWindow->isOpen()) {
		// keep 60 fps
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
		chrono::microseconds sleep_time = chrono::microseconds(16667) - chrono::duration_cast<chrono::microseconds>(last_frame - now);
		this_thread::sleep_for(sleep_time);
	}
	
	return 0;
}