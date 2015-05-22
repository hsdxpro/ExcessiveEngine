#include "Awesomium/WebCore.h"
#include "Awesomium/BitmapSurface.h"
#include "Awesomium/STLHelpers.h"

#include <thread>
#include "../Common/src/IWindow.h"
#include "../Common/src/Factory.h"
#include "../Common/src/EngineCore.h"

// Various macro definitions
#define WIDTH   800
#define HEIGHT  600
#define URL     "http://www.google.com"

//using namespace Awesomium;

static const char vertexShaderCode[] =
/**
"#version 440 core \n"
"layout(std140) uniform constant_data \n"
"{ \n"
"  mat4 mvp; \n"
"} cd; \n"
"layout(location = 0) in vec3 in_vertex; \n"
"void main() \n"
"{ \n"
"  gl_Position = cd.mvp * vec4(in_vertex, 1); \n"
"} \n";
*/

"#version 440 core \n"
"in vec3 in_vertex; \n"
"void main() \n"
"{ \n"
"	gl_Position = vec4(in_vertex.xy,0.0,1.0);\n"
"} \n";


static const char pixelShaderCode[] =
"#version 440 core \n"
"uniform sampler2D tex; \n"

"in vec3 pos; \n"
"out vec4 color; \n"
"void main() \n"
"{ \n"
"   color = vec4(1,0,0,1);\n"
"} \n"
;

int main() 
{
	// Init window
	rWindow d;
		d.clientW = 800;
		d.clientH = 600;
		d.capText = "Excessive Awesome Editor";
	IWindow* window = Factory::createWindow(d);

	// Init engine core (graphics, physics, sound, network
	EngineCore core;
		//core.initSoundEngine();
		//core.initNetworkEngine();
		//core.initPhysicsEngineBullet();
	rGraphicsEngineRaster gDesc;
	gDesc.type = eGapiType::OPENGL_4_5;
	graphics::IEngine* gEngine = core.initGraphicsEngineRaster(gDesc);

	gEngine->setResolution(window->getClientW(), window->getClientH());

// GAPI HACKED THINGS
	auto gapi = gEngine->getGapi();

	// Shader for fsq render
	auto shaderAwesomium = gapi->createShaderSource(vertexShaderCode, pixelShaderCode);

	// new vertex attribs (doesn't work yet)
	rInputElement elements[1];
		elements[0].setName("in_vertex");
		elements[0].num_components = 3;
		elements[0].offset = 0;
		elements[0].type = eVertexAttribType::FLOAT;
		elements[0].stream_index = 0;

	IInputLayout* input_layout = gapi->createInputLayout(elements, 1);

	mm::vec3 vertices[6];
	vertices[0] = mm::vec3(0, 0, 0);
	vertices[1] = mm::vec3(1, 1, 0);
	vertices[2] = mm::vec3(1, 0, 0);
	vertices[3] = mm::vec3(0, 0, 0);
	vertices[4] = mm::vec3(0, 0, 0);
	vertices[5] = mm::vec3(0, 0, 0);


	// vertex buffer(s)
	rBuffer vbDesc;
		vbDesc.is_readable = false;
		vbDesc.is_writable = false;
		vbDesc.is_persistent = true;
		vbDesc.prefer_cpu_storage = false;
		vbDesc.size = elements[0].num_components * sizeof(float) * 6;
		vbDesc.initial_data = vertices;
	auto vb0 = gapi->createVertexBuffer(vbDesc);

	u32 indices[6] = { 0, 1, 2, 3, 4, 5 };

	rBuffer ibDesc;
		ibDesc.is_readable = false;
		ibDesc.is_writable = true;
		ibDesc.is_persistent = false;
		ibDesc.prefer_cpu_storage = false;
		ibDesc.size = 6 * sizeof(u32);
		ibDesc.initial_data = indices;
	auto ib = gapi->createIndexBuffer(ibDesc);
// (GAPI HACKED THINGS) END
	// Create the WebCore singleton with default configuration
	Awesomium::WebCore* web_core = Awesomium::WebCore::Initialize(Awesomium::WebConfig());

	// Create a new WebView instance with a certain width and height
	Awesomium::WebView* view = web_core->CreateWebView(WIDTH, HEIGHT);

	// Load a certain URL into our WebView instance
	Awesomium::WebURL url(Awesomium::WSLit(URL));
	view->LoadURL(url);

	// Wait for our WebView to finish loading
	

	while (window->isOpen())
	{
		rWindowEvent evt;
		while (window->popEvent(&evt))
		{
			//if (evt.msg == eWindowMsg::CLOSE)
			//	window->close()
			//...
		}

		//while (view->IsLoading())
		//	web_core->Update();
		//
		//web_core->Update();
		//gapi->clearFrameBuffer(eClearFlag::COLOR_DEPTH_STENCIL, mm::vec4(0, 0, 0, 1));

		// Get the WebView's rendering Surface. The default Surface is of
		// type 'BitmapSurface', we must cast it before we can use it.
		//Awesomium::BitmapSurface* surface = (Awesomium::BitmapSurface*)view->surface();

		gapi->clearFrameBuffer(eClearFlag::COLOR_DEPTH, mm::vec4(0, 0, 0, 0), 1, 0);

		rDepthState ds;
		ds.enable_test = false;
		ds.enable_write = true;
		ds.far = 1.0f;
		ds.near = 0.0f;
		ds.func = eCompareFunc::ALWAYS;
		gapi->setDepthState(ds);

		// Set input layout, vertex buffers
		gapi->setInputLayout(input_layout);

		u32 strides = 12;// [2] = { 12, 8 };
		u32 offsets = 0;// [2] = { 0, 0, };
		gapi->setVertexBuffers(&vb0, &strides, &offsets, 0, 1);
		gapi->setIndexBuffer(ib);
		//gapi->setVertexBuffers(&vb1, &strides[1], &offsets[1], 0, 1);
		gapi->setShaderProgram(shaderAwesomium);
		gapi->setRenderTargets(0, 0);

		gapi->draw(6);

		// Kirenderelõdik az egész világ szépen ahogy van, és ennek a tetejére szeretnénk renderelni
		// egyelõre opengl framebufferére is rámásolhatnánk

		// Vagy csinálunk egy olyan entity - t
		window->displayClientRect();
	}

	// Make sure our surface is not NULL-- it may be NULL if the WebView 
	// process has crashed.
	//if (surface != 0) {
	//	// Save our BitmapSurface to a JPEG image in the current
	//	// working directory.
	//	surface->SaveToJPEG(Awesomium::WSLit("./result.jpg"));
	//}

	return 0;
}