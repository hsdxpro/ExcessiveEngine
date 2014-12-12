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


static const char vertexShaderCode[] =
"#version 440 core \n"
"layout(location = 0) in vec3 in_vertex; \n"
"void main() \n"
"{ \n"
"  mat3 mz = mat3("
"  cos(0.78f), -sin(0.78f), 0,"
"  sin(0.78f), cos(0.78f), 0,"
"  0, 0, 1);"
"  mat3 mx = mat3("
"  1, 0, 0,"
"  0, cos(0.78f), -sin(0.78f),"
"  0, sin(0.78f), cos(0.78f));"
"  vec3 pos = mx * mz * (in_vertex * 0.2f); \n"
"  gl_Position = vec4(pos, pos.z+1);\n"
"} \n"
;

static const char pixelShaderCode[] =
"#version 440 core \n"
"out vec4 color; \n"
"void main() \n"
"{ \n"
"  color = vec4(0.8f, 0.8f, 0.8f, 0.2f); \n"
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
	rShaderSources sources;
	sources.psSrc = pixelShaderCode;
	sources.vsSrc = vertexShaderCode;
	shader = gapi->createShaderProgram(sources);

	unsigned index = shader->getAttributeIndex("in_vertex");
	cout << index;

	gapi->setDebugOutput(true);
	gapi->setSeamlessCubeMaps(true);
	gapi->setSyncDebugOutput(true);

	isValid = shader != nullptr;
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


////////////////////////////////////////////////////////////////////////////////
// update
void GraphicsEngineRaster::update() {
	cout << "Updating frame..." << endl;

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

	cout << entities.size() << " entities to be drawn..." << endl;

	// mesh type store
	static std::unordered_map<uint64_t, rVertexAttrib> meshFormats;

	// for each entity
	int num_drawn = 0;
	for (auto entity : entities) {
		// get mesh
		Mesh* mesh = entity->getMesh();
		if (!mesh) {
			cout << "Entity has no mesh :(" << endl;
			continue;
		}
		uint64_t meshId = mesh->getElementConfigId();
		rVertexAttrib attrib;

		Mesh::ElementInfo posInfo;
		bool hasPosition;
		hasPosition = mesh->getElementBySemantic(posInfo, Mesh::POSITION);

		auto it = meshFormats.find(meshId);
		if (it == meshFormats.end()) {
			if (!hasPosition) {
				cout << "This mesh does not have position... KILL IT WITH FIRE!" << endl;
				continue;
			}

			attrib.index = shader->getAttributeIndex("in_vertex");
			attrib.nComponent = posInfo.num_components;
			attrib.offset = posInfo.offset;
			attrib.type = eVertexAttribType::FLOAT;
			attrib.size = attrib.divisor = 0;

			meshFormats.insert(std::pair<uint64_t, rVertexAttrib>(meshId, attrib));
		}
		else {
			attrib = it->second;
		}

		// set stuff
		gapi->setViewport(0, 0, 512, 512);
		gapi->setShaderProgram(shader);
		gapi->setRenderTargets(0, 0);

		// set vertex buffer
		gapi->setVertexBuffers(&posInfo.buffer, &attrib, 1);
		// set index buffer
		auto ib = mesh->getIndexBuffer();
		gapi->setIndexBuffer(mesh->getIndexBuffer());

		// draw
		unsigned num_indices = mesh->getIndexBuffer()->getDesc().size / sizeof(uint32_t);
		gapi->draw(num_indices);
		rDepthState ds;
		ds.enable_test = false;
		ds.enable_write = false;
		ds.far = 1.0f;
		ds.near = 0.0f;
		ds.func = eCompareFunc::ALWAYS;
		gapi->setDepthState(ds);
		num_drawn++;
	}

	cout << num_drawn << " entities actually drawn." << endl;
}

IGapi* GraphicsEngineRaster::getGapi() {
	return gapi;
}