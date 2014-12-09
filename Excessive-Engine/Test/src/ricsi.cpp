#include "tests.h"
#include "Factory.h"
#include "IWindow.h"
#include "IGraphicsEngine.h"
#include "..\GraphicsApi_Interface\interface\Common.h"
#include "..\GraphicsApi_Interface\interface\IShaderProgram.h"
#include "..\GraphicsApi_Interface\interface\IGapi.h"


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


ge::IGraphicsEngine*	gEngine;
IGapi*					gGapi;
IWindow*				gWindow;

int Ricsi() {

	// Init GraphicsEngine
	ge::rGraphicsEngine gDesc;
		gDesc.gapi = Factory::createGapiGL();
	gEngine = Factory::createGraphicsEngineRaster(gDesc);

	// Get the API belonging to graphics engine
	gGapi = gEngine->getGapi();

	// Init window
	rWindow d;
		d.clientW = 800;
		d.clientH = 600;
		d.capText = "Excessive-Engine -> Ricsi teszt";
	gWindow = Factory::createWindow(d);

	// Create simple shaderProgram
	rShaderProgSources s;
		s.vsSrc = vsSimple;
		s.psSrc = psSimple;
	//gGapi->createShaderProgram(s);

	std::string asd= Sys::getWorkDir();

	// Create texture that we draw as FSQ
	//rTextureDesc d;
	//ITexture* tex = gGapi->createTexture(Sys::getWorkDir() + "image.png");
	//
	//string image_path = app_path + "image.png";
	//sf::Image im;
	//im.loadFromFile(image_path);
	//
	//ITexture::rDesc texdata;
	//texdata.width = im.getSize().x;
	//texdata.height = im.getSize().y;
	//texdata.depth = 1;
	//texdata.format = RGBA8;
	//texdata.is_cubemap = false;
	//texdata.is_layered = false;
	//texdata.num_levels = 1;
	//
	//auto tex = gapi->createTexture(texdata);
	//
	//rTextureUpdateData texupdata;
	//texupdata.data = (char*)im.getPixelsPtr();
	//texupdata.depth = texdata.depth;
	//texupdata.format = texdata.format;
	//texupdata.width = texdata.width;
	//texupdata.height = texdata.height;
	//texupdata.level = 0;
	//texupdata.x_offset = 0;
	//texupdata.y_offset = 0;
	//texupdata.z_offset = 0;
	//
	//tex->update(&texupdata);

	rWindowEvent ev;
	while (gWindow->isOpen()) {
		while (gWindow->popEvent(&ev))
			if (ev.msg == eWindowMsg::KEY_PRESS && ev.key == eKey::ESCAPE)
				gWindow->close();

		// Update graphics engine
		gEngine->update();

		// Call that after OpenGL "finish" all of it's rendering
		gWindow->displayClientRect();
		
	}
	
	return 0;
}