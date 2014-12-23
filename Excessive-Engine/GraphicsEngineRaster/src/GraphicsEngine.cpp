#include "GraphicsEngine.h"
#include <GraphicsApi>

#include <iostream> // only for debug
#include <unordered_map> // for crappy test render
#include "..\Common\src\Factory.h"
using std::cout;
using std::endl;


#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

//
ITexture* gTmpTex;

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
"layout(std140) uniform constant_data \n"
"{ \n"
"  mat4 mvp; \n"
"} cd; \n"
"in vec3 in_vertex; \n"
"in vec3 in_normal; \n"
"in vec2 in_tex0; \n"
"out vec3 pos; \n"
"out vec3 normal; \n"
"out vec2 tex0; \n"
"void main() \n"
"{ \n"
"	pos = vec3(in_vertex.x, in_vertex.y, in_vertex.z); \n"
"	normal = in_normal; \n"
"	tex0 = in_tex0; \n"
//"	normal.x = in_normal.x*2.0f + 1.0f; \n"
//"	normal.y = in_normal.y*4.0f - 2.0f; \n"
//"	normal.z = float(normal.y < 0.0f)*2.0f - 0.5f; \n"
//"	normal.y += normal.z * 1.0f; \n"
//"	normal.z *= sqrt(1 - normal.x*normal.x - normal.y*normal.y); \n"
"	gl_Position = cd.mvp * vec4(pos, 1);\n"
"} \n";


static const char pixelShaderCode[] =
"#version 440 core \n"
"uniform sampler2D tex; \n"

"in vec3 pos; \n"
"in vec3 normal; \n"
"in vec2 tex0; \n"
"out vec4 color; \n"
"void main() \n"
"{ \n"
"	float intensity = dot(normalize(normal), normalize(vec3(1, 1, 1))); \n"
"   color = texture(tex, tex0); \n"
"} \n"
;


////////////////////////////////////////////////////////////////////////////////
// Export Create function
//
////////////////////////////////////////////////////////////////////////////////

extern "C"
EXPORT graphics::IGraphicsEngine* CreateGraphicsEngine(const graphics::rGraphicsEngine& d) {
	auto myEngine = new GraphicsEngineRaster(d);
	if (myEngine->isConstructionSucceeded()) {
		return myEngine;
	}
	else {
		delete myEngine;
		return nullptr;
	}
}



////////////////////////////////////////////////////////////////////////////////
// GraphicsEngine class
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ctor, dtor, release

GraphicsEngineRaster::GraphicsEngineRaster(const graphics::rGraphicsEngine& d) {
	gapi = d.gapi;

	// WARNING: temporary testing code
	// create shaders
	shader = gapi->createShaderSource(vertexShaderCode, pixelShaderCode);
	isValid = shader != nullptr;


	gTmpTex = gapi->createTexture("D:/D_32/image.png");
	return;

	u32 index = shader->getAttributeIndex("in_vertex");
	cout << index;

	gapi->setDebugOutput(true);
	gapi->setSeamlessCubeMaps(true);
	gapi->setSyncDebugOutput(true);
}

GraphicsEngineRaster::~GraphicsEngineRaster() {
	for (auto scene : scenes) {
		delete scene;
	}
	/*
	meshes and materials should be freed externally
	*/
}


void GraphicsEngineRaster::release() {
	delete this;
}


////////////////////////////////////////////////////////////////////////////////
// create stuff
Scene* GraphicsEngineRaster::createScene() {
	Scene* s = new Scene;
	scenes.insert(s);
	return s;
}

void GraphicsEngineRaster::deleteScene(graphics::IScene* scene) {
	auto it = scenes.find((Scene*)scene);
	if (it != scenes.end()) {
		delete *it;
		scenes.erase(it);
	}
}

Mesh* GraphicsEngineRaster::createMesh() {
	return new Mesh(gapi);
}

Material* GraphicsEngineRaster::createMaterial() {
	return new Material;
}

Texture* GraphicsEngineRaster::createTexture() {
	return new Texture;
}

Camera* GraphicsEngineRaster::createCam() {
	return new Camera();
}

////////////////////////////////////////////////////////////////////////////////
// update
void GraphicsEngineRaster::update() {
	//cout << "Updating frame..." << endl;

	// ok, this function is only for testing purposes, it's not a real renderer xD	
	// just render the first scene, entity by entity

	if (scenes.empty()) {
		cout << "No scene were found :(" << endl;
		return;
	}

	// get entities and lights
	Scene& scene = **scenes.begin();
	
	auto entities = scene.getEntities();
	auto lights = scene.getLights();

	//cout << entities.size() << " entities to be drawn..." << endl;


	// for each entity
	int num_drawn = 0;
	for (auto entity : entities) {
		// get mesh
		Mesh* mesh = entity->getMesh();
		if (!mesh) {
			//cout << "Entity has no mesh :(" << endl;
			continue;
		}
		rVertexAttrib attribs[3];
		Mesh::ElementInfo attribInfos[3];
		//bool hasPosition;
		mesh->getElementBySemantic(attribInfos[0], Mesh::POSITION);
		mesh->getElementBySemantic(attribInfos[1], Mesh::NORMAL);
		mesh->getElementBySemantic(attribInfos[2], Mesh::TEX0);
		//if (!hasPosition) {
		//	//cout << "This mesh does not have position... KILL IT WITH FIRE!" << endl;
		//	continue;
		//}

		attribs[0].index = shader->getAttributeIndex("in_vertex");
		attribs[0].nComponent = attribInfos[0].num_components;
		attribs[0].offset = attribInfos[0].offset;;
		attribs[0].type = eVertexAttribType::FLOAT;
		attribs[0].size = 0;
		attribs[0].divisor = 0;

		attribs[1].index = shader->getAttributeIndex("in_normal");
		attribs[1].nComponent = attribInfos[1].num_components;
		attribs[1].offset = attribInfos[1].offset;
		attribs[1].type = eVertexAttribType::FLOAT;
		attribs[1].size = 0;
		attribs[1].divisor = 0;

		attribs[2].index = shader->getAttributeIndex("in_tex0");
		attribs[2].nComponent = attribInfos[2].num_components;
		attribs[2].offset = attribInfos[2].offset;
		attribs[2].type = eVertexAttribType::FLOAT;
		attribs[2].size = 0;
		attribs[2].divisor = 0;


		// set stuff
		gapi->setViewport(0, 0, 800, 600);
		gapi->setShaderProgram(shader);
		gapi->setRenderTargets(0, 0);

		gapi->setTexture(gTmpTex, shader->getSamplerIndex("tex"));

		mm::mat4 wvp = scene.getCam()->getProjMatrix() * scene.getCam()->getViewMatrix();

		rBuffer ubo_alloc_data;
			ubo_alloc_data.is_persistent = false;
			ubo_alloc_data.is_readable = true;
			ubo_alloc_data.is_writable = true;
			ubo_alloc_data.prefer_cpu_storage = false;
			ubo_alloc_data.size = 1 * sizeof(mm::mat4);
		auto ubo_buf = gapi->createUniformBuffer(ubo_alloc_data);
		ubo_buf->update((char*)&wvp[0][0], sizeof(mm::mat4), 0);
		gapi->setUniformBuffer(ubo_buf, shader->getUniformBlockIndex("constant_data"));

		// set vertex buffer
		IVertexBuffer* tmp[3] = { attribInfos[0].buffer, attribInfos[1].buffer, attribInfos[2].buffer };
		gapi->setVertexBuffers(tmp, attribs, 3);

		// set index buffer
		auto ib = mesh->getIndexBuffer();
		gapi->setIndexBuffer(mesh->getIndexBuffer());

		// draw
		u32 num_indices = mesh->getIndexBuffer()->getDesc().size / sizeof(u32);
		gapi->draw(num_indices);
		rDepthState ds;
		ds.enable_test = true;
		ds.enable_write = true;
		ds.far = 1.0f;
		ds.near = 0.0f;
		ds.func = eCompareFunc::LESSER_OR_EQUAL;
		gapi->setDepthState(ds);
		num_drawn++;

		ubo_buf->destroy();
	}

	//cout << num_drawn << " entities actually drawn." << endl;
}

IGapi* GraphicsEngineRaster::getGapi() {
	return gapi;
}