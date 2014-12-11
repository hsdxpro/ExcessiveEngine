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


////////////////////////////////////////////////////////////////////////////////
// Export Create function
//
////////////////////////////////////////////////////////////////////////////////

extern "C"
EXPORT graphics::IGraphicsEngine* CreateGraphicsEngine(const graphics::rGraphicsEngine& d) {
	return new GraphicsEngineRaster(d);
}


////////////////////////////////////////////////////////////////////////////////
// GraphicsEngine class
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// ctor, dtor, release

GraphicsEngineRaster::GraphicsEngineRaster(const graphics::rGraphicsEngine& d) {
	gapi = d.gapi;
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
		}
		uint64_t meshId = mesh->getElementConfigId();
		rVertexAttrib attrib;

		Mesh::ElementInfo posInfo;
		bool hasPosition;
		hasPosition == mesh->getElementBySemantic(posInfo, Mesh::POSITION);

		auto it = meshFormats.find(meshId);
		if (it == meshFormats.end()) {
			if (!hasPosition) {
				cout << "This mesh does not have position... KILL IT WITH FIRE!" << endl;
				continue;
			}

			attrib.index = 0;
			attrib.nComponent = posInfo.num_components;
			attrib.offset = posInfo.offset;
			attrib.type = eVertexAttribType::FLOAT;
			attrib.size = attrib.divisor = 0;

			meshFormats.insert(std::pair<uint64_t, rVertexAttrib>(meshId, attrib));
		}
		else {
			attrib = it->second;
		}

		// set vertex buffer
		gapi->setVertexBuffers(&posInfo.buffer, &attrib, 1);
		// set index buffer
		gapi->setIndexBuffer(mesh->getIndexBuffer());

		// draw
		unsigned num_indices = mesh->getIndexBuffer()->getDesc().size / sizeof(uint32_t);
		gapi->draw(num_indices);
		num_drawn++;
	}

	cout << num_drawn << " entities actually drawn." << endl;
}

IGapi* GraphicsEngineRaster::getGapi() {
	return gapi;
}