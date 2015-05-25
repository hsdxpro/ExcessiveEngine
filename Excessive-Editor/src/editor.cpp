#include "Awesomium/WebCore.h"
#include "Awesomium/BitmapSurface.h"
#include "Awesomium/STLHelpers.h"

#include "../Common/src/IWindow.h"
#include "../Common/src/Factory.h"
#include "../Common/src/EngineCore.h"

static const char vertexShaderCode[] =
"#version 440 core \n"
"const vec2 quadVertices[3] = { vec2(-1, -1), vec2(3, -1), vec2(-1, 3) };\n"
"const vec2 quadTexcoords[3] = { vec2(0, 1), vec2(2, 1), vec2(0, -1) };\n"
"out vec2 tex0; \n"
"void main()\n"
"{\n"
"	gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);\n"
"	tex0 = quadTexcoords[gl_VertexID]; \n"
"}\n";

static const char pixelShaderCode[] =
"#version 440 core \n"
"uniform sampler2D tex; \n"
"in vec3 pos; \n"
"in vec2 tex0; \n"
"out vec4 color; \n"
"void main() \n"
"{ \n"
"   color = texture(tex, tex0);\n"
"} \n"
;

EngineCore gEngineCore;

int main() 
{
	// Window
	rWindow d;
		d.clientW = 800;
		d.clientH = 600;
		d.capText = "Excessive Awesome Editor";
	IWindow* window = Factory::createWindow(d);

	// Engine core
	rGraphicsEngineRaster gDesc;
		gDesc.gapiType = eGapiType::OPENGL_4_5;
		gDesc.targetWindow = window;
	graphics::IEngine* gEngine = gEngineCore.initGraphicsEngineRaster(gDesc);

// GAPI HACKED THINGS
	auto gapi = gEngine->getGapi();

	// Shader for fsq render
	auto shaderAwesomium = gapi->createShaderSource(vertexShaderCode, pixelShaderCode);

// Awesome...
	Awesomium::WebCore* web_core = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
	Awesomium::WebView* view = web_core->CreateWebView(window->getClientW(), window->getClientH());
	Awesomium::WebURL url(Awesomium::WSLit("http://www.google.com"));
	view->LoadURL(url);
	
	while (view->IsLoading())
		web_core->Update();

	// Texture we render awesomium into
	rTextureGapi texDesc;
		texDesc.depth = 1;
		texDesc.format = eTextureFormat::RGBA8;
		texDesc.height = window->getClientH();
		texDesc.width = window->getClientW();
		texDesc.is_cubemap = false;
		texDesc.is_layered = false;
		texDesc.num_levels = 1;
	ITextureGapi* texAwesome = gapi->createTexture(texDesc);

	void* tmpAwesomiumSurfaceData = malloc(window->getClientW() * window->getClientH() * 4);

	while (window->isOpen())
	{
		rWindowEvent evt;
		while (window->popEvent(&evt))
		{
			if (evt.msg == eWindowMsg::MOUSE_MOVE)
			{
				view->InjectMouseMove(evt.x, evt.y);
			}
			else if (evt.msg == eWindowMsg::MOUSE_PRESS)
			{
				if (evt.mouseBtn == eMouseBtn::LEFT)
					view->InjectMouseDown(Awesomium::kMouseButton_Left);
				else if (evt.mouseBtn == eMouseBtn::MIDDLE)
					view->InjectMouseDown(Awesomium::kMouseButton_Middle);
				else if (evt.mouseBtn == eMouseBtn::RIGHT)
					view->InjectMouseDown(Awesomium::kMouseButton_Right);
			}
			else if (evt.msg == eWindowMsg::MOUSE_RELEASE)
			{
				if (evt.mouseBtn == eMouseBtn::LEFT)
					view->InjectMouseUp(Awesomium::kMouseButton_Left);
				else if (evt.mouseBtn == eMouseBtn::MIDDLE)
					view->InjectMouseUp(Awesomium::kMouseButton_Middle);
				else if (evt.mouseBtn == eMouseBtn::RIGHT)
					view->InjectMouseUp(Awesomium::kMouseButton_Right);
			}
			else if (evt.msg == eWindowMsg::MOUSE_WHEEL)
			{
				view->InjectMouseWheel(evt.deltaY * 100, 0);
			}
			else if (evt.msg == eWindowMsg::RESIZE)
			{
				tmpAwesomiumSurfaceData = realloc(tmpAwesomiumSurfaceData, evt.x * evt.y * 4);
				view->Resize(evt.x, evt.y);
				gapi->setViewport(0, 0, evt.x, evt.y);

				texAwesome->destroy();

				rTextureGapi texDesc;
					texDesc.depth = 1;
					texDesc.format = eTextureFormat::RGBA8;
					texDesc.height = evt.y;
					texDesc.width = evt.x;
					texDesc.is_cubemap = false;
					texDesc.is_layered = false;
					texDesc.num_levels = 1;
				texAwesome = gapi->createTexture(texDesc);
			}
		}

		web_core->Update();

		Awesomium::BitmapSurface* surface = (Awesomium::BitmapSurface*)view->surface();
		
		if (surface->is_dirty())
		{
			surface->CopyTo((u8*)tmpAwesomiumSurfaceData, window->getClientW() * 4, 4, false, false);
			rTextureUpdate texUpdate;
				texUpdate.data = tmpAwesomiumSurfaceData;
				texUpdate.depth = 1;
				texUpdate.format = eTextureFormat::RGBA8;
				texUpdate.height = window->getClientH();
				texUpdate.width = window->getClientW();
				texUpdate.level = 0;
			gapi->writeTexture(texAwesome, texUpdate);
		}

		gapi->setTexture(texAwesome, shaderAwesomium->getSamplerIndex("tex"));

		gapi->clearFrameBuffer(eClearFlag::COLOR_DEPTH_STENCIL, mm::vec4(0, 1, 0, 1), 1);

		rDepthState ds;
			ds.enable_test = false;
		gapi->setDepthState(ds);

		gapi->setShaderProgram(shaderAwesomium);
		gapi->setRenderTargets(0, 0);

		gapi->draw(3);

		window->present();
	}

	Awesomium::BitmapSurface* surface = (Awesomium::BitmapSurface*)view->surface();
	// Make sure our surface is not NULL-- it may be NULL if the WebView 
	// process has crashed.
	if (surface != 0) {
		// Save our BitmapSurface to a JPEG image in the current
		// working directory.
		surface->SaveToJPEG(Awesomium::WSLit("./result.jpg"));
	}

	// Clean up
	free(tmpAwesomiumSurfaceData);
	view->Destroy();
	Awesomium::WebCore::Shutdown();

	return 0;
}