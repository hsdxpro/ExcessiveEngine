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

"layout(std140) uniform ps_const \n"
"{ \n"
"	vec4 diffuse; \n"
"	int hasTex; \n"
"} pscd; \n"

"in vec3 pos; \n"
"in vec3 normal; \n"
"in vec2 tex0; \n"
"out vec4 color; \n"
"void main() \n"
"{ \n"
"	float intensity = clamp(dot(normalize(normal), normalize(vec3(1, 1, 1))), 0, 1); \n"
"	const float t = 0.7;"
"	const vec4 sun_color = vec4(1.0, 0.92, 0.72, 1);"
"	const vec4 sky_color = vec4(0.6, 0.8, 1.0, 1);"

//"if (pscd.hasTex != 0) { \n"
"	color = pscd.diffuse*texture(tex, tex0); \n"
//"} \n"
//"else { \n"
//"	color = pscd.diffuse; \n"
//"} \n"
//"	color = clamp(color,0,1) + vec4(0.3,0,0,0); \n"

"   color = color*t*intensity*sun_color + color*(1-t)*sky_color;\n"
"} \n"
;


////////////////////////////////////////////////////////////////////////////////
// Export Create function
//
////////////////////////////////////////////////////////////////////////////////

extern "C"
EXPORT graphics::IGraphicsEngine* CreateGraphicsEngineRaster(const rGraphicsEngineRaster& d) {
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

GraphicsEngineRaster::GraphicsEngineRaster(const rGraphicsEngineRaster& d) {
	gapi = Factory::createGapiGL();

	// WARNING: temporary testing code
	// create shaders
	shader = gapi->createShaderSource(vertexShaderCode, pixelShaderCode);
	isValid = shader != nullptr;

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
	return new Texture(gapi);
}

Camera* GraphicsEngineRaster::createCam() {
	return new Camera();
}

////////////////////////////////////////////////////////////////////////////////
// update
void GraphicsEngineRaster::update() {
	//cout << "Updating frame..." << endl;

	gapi->clearFrameBuffer(eClearFlag::COLOR_DEPTH, mm::vec4(0, 0, 0, 0), 0, 0);

	rDepthState ds;
		ds.enable_test = true;
		ds.enable_write = true;
		ds.far = 1.0f;
		ds.near = 0.0f;
		ds.func = eCompareFunc::LESSER_OR_EQUAL;
	gapi->setDepthState(ds);

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
		attribs[0].offset = attribInfos[2].offset;
		attribs[0].type = eVertexAttribType::FLOAT;
		attribs[0].size = 0;
		attribs[0].divisor = 0;

		attribs[1].index = shader->getAttributeIndex("in_normal");
		attribs[1].nComponent = attribInfos[1].num_components;
		attribs[1].offset = attribInfos[2].offset;
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

		mm::mat4 wvp = scene.getCam()->getProjMatrix() * scene.getCam()->getViewMatrix();

		rBuffer ubo_alloc_data;
			ubo_alloc_data.is_persistent = false;
			ubo_alloc_data.is_readable = true;
			ubo_alloc_data.is_writable = true;
			ubo_alloc_data.prefer_cpu_storage = false;
			ubo_alloc_data.size = 1 * sizeof(mm::mat4);
		auto ubo_buf = gapi->createUniformBuffer(ubo_alloc_data);
		auto index_vs = shader->getUniformBlockIndex("constant_data");

		ubo_buf->update(&wvp, sizeof(mm::mat4), 0);
		gapi->setUniformBuffer(ubo_buf, index_vs);

		// set vertex buffer
		IVertexBuffer* tmp[3] = { attribInfos[0].buffer, attribInfos[1].buffer, attribInfos[2].buffer };
		gapi->setVertexBuffers(tmp, attribs, 3);

		// set index buffer
		auto ib = mesh->getIndexBuffer();
		gapi->setIndexBuffer(mesh->getIndexBuffer());

		// set material
		Material* mtl = entity->getMaterial();
		mm::vec4 diffuse;
		Texture* tdiffuse = nullptr;

		struct {
			mm::vec4 diffuse;
			int hasTex;
		} ps_const;



		rBuffer pc_const_desc;
			pc_const_desc.is_persistent = false;
			pc_const_desc.is_readable = true;
			pc_const_desc.is_writable = true;
			pc_const_desc.prefer_cpu_storage = false;
			pc_const_desc.size = sizeof(ps_const);
		auto ps_const_buf = gapi->createUniformBuffer(pc_const_desc);
		auto index_ps = shader->getUniformBlockIndex("ps_const");

		// Draw each subMaterial
		if (mtl != nullptr)
		for (int i = 0; i < mtl->getNumSubMaterials(); i++) {
			auto& subMat = mtl->getSubMaterial(i);

			if (subMat.t_diffuse) {
				diffuse = subMat.base;
				tdiffuse = (Texture*)subMat.t_diffuse;

				ps_const.diffuse = diffuse;
				ps_const.hasTex = (tdiffuse != nullptr && tdiffuse->getTexture() != nullptr);
			}
			else {
				ps_const.diffuse = mm::vec4(1, 1, 1, 1);
				ps_const.hasTex = false;
			}

			gapi->setUniformBuffer(ps_const_buf, index_ps);
			ps_const_buf->update(&ps_const, sizeof(ps_const), 0);

			if (ps_const.hasTex) {
				gapi->setTexture(tdiffuse->getTexture(), shader->getSamplerIndex("tex"));
			}

			auto& matGroup = mesh->getMaterialIds()[i];

			// draw
			gapi->draw((matGroup.endFace - matGroup.beginFace) * 3, matGroup.beginFace * 3 * sizeof(u32));

			num_drawn++;
		}
		ps_const_buf->destroy();
		ubo_buf->destroy();
	}

	//cout << num_drawn << " entities actually drawn." << endl;
}

IGapi* GraphicsEngineRaster::getGapi() {
	return gapi;
}