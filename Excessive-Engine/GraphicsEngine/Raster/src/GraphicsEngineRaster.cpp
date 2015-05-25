#include "GraphicsEngineRaster.h"

#include <iostream> // only for debug
#include <unordered_map>
#include <map>
#include "..\Common\src\Factory.h"
#include "mymath\mm_quat_func.h"
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
"	if (color.a < 0.5f) discard; \n"
"   color = color*t*intensity*sun_color + color*(1-t)*sky_color;\n"
"} \n"
;


////////////////////////////////////////////////////////////////////////////////
// Export Create function
//
////////////////////////////////////////////////////////////////////////////////

extern "C"
EXPORT graphics::IEngine* CreateGraphicsEngineRaster(const rGraphicsEngineRaster& d) {
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

	u32 windowWidth = d.targetWindow->getClientW();
	u32 windowHeight = d.targetWindow->getClientH();
	gapi->setViewport(	windowWidth * d.renderRegion.bottomLeftPercentNormed.x,
						windowHeight * d.renderRegion.bottomLeftPercentNormed.y,
						windowWidth * (d.renderRegion.topRightPercentNormed.x - d.renderRegion.bottomLeftPercentNormed.x),
						windowHeight * (d.renderRegion.topRightPercentNormed.y - d.renderRegion.bottomLeftPercentNormed.y));

	gapi->setDebugOutput(true);
	gapi->setSeamlessCubeMaps(true);
	gapi->setSyncDebugOutput(true);
}

GraphicsEngineRaster::~GraphicsEngineRaster() {

}


void GraphicsEngineRaster::release() {
	delete this;
}


////////////////////////////////////////////////////////////////////////////////
// create stuff
Scene* GraphicsEngineRaster::createScene() {
	Scene* s = new Scene;
	return s;
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
// scene system

void GraphicsEngineRaster::addLayer(const Layer& layer) {
	layers.push_back(layer);
}

void GraphicsEngineRaster::removeLayer(size_t index) {
	assert(index < getNumLayers());
	layers.erase(layers.begin() + index);
}

size_t GraphicsEngineRaster::getNumLayers() const {
	return layers.size();
}

void GraphicsEngineRaster::setNumLayers(size_t num_layers) {
	layers.resize(num_layers);
}

auto GraphicsEngineRaster::getLayer(size_t index) -> Layer& {
	assert(index < getNumLayers());
	return layers[index];
}



////////////////////////////////////////////////////////////////////////////////
// update
void GraphicsEngineRaster::update(float deltaTime) {
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

	if (layers.empty() || !layers.begin()->scene) {
		cout << "No scene were found :(" << endl;
		return;
	}

	// get entities and lights
	Scene& scene = *((Scene*)layers.begin()->scene);
	
	auto entities = scene.getEntities();
	auto lights = scene.getLights();


	// for each entity
	int num_drawn = 0;
	for (auto entity : entities) {
		// get mesh
		Mesh* mesh = entity->getMesh();
		if (!mesh) {
			continue;
		}
		rVertexAttrib attribs[3];
		Mesh::ElementInfo attribInfos[3];
		bool hasAllAttribs =
			mesh->getElementBySemantic(attribInfos[0], Mesh::POSITION) &&
			mesh->getElementBySemantic(attribInfos[1], Mesh::NORMAL) &&
			mesh->getElementBySemantic(attribInfos[2], Mesh::TEX0);
		//if (!hasAllAttribs) {
		//	continue;
		//}

		// create input layout
		auto ConvertType = [](Mesh::ElementType type)->eVertexAttribType {
			switch (type) {
				case Mesh::FLOAT: return eVertexAttribType::FLOAT;
				case Mesh::HALF: return eVertexAttribType::HALF;
				case Mesh::SINT_32: return eVertexAttribType::SINT_32;
				case Mesh::UINT_32: return eVertexAttribType::UINT_32;
				case Mesh::SINT_16: return eVertexAttribType::SINT_16;
				case Mesh::UINT_16: return eVertexAttribType::UINT_16;
				case Mesh::SINT_8: return eVertexAttribType::SINT_8;
				case Mesh::UINT_8: return eVertexAttribType::UINT_8;
				case Mesh::SNORM_32: return eVertexAttribType::SNORM_32;
				case Mesh::UNORM_32: return eVertexAttribType::UNORM_32;
				case Mesh::SNORM_16: return eVertexAttribType::SNORM_16;
				case Mesh::UNORM_16: return eVertexAttribType::UNORM_16;
				case Mesh::SNORM_8: return eVertexAttribType::SNORM_8;
				case Mesh::UNORM_8: return eVertexAttribType::UNORM_8;
				default: return eVertexAttribType::FLOAT;
			}
		};

		// new vertex attribs (doesn't work yet)
		rInputElement elements[3];
		elements[0].setName("in_vertex");
		elements[0].num_components = attribInfos[0].num_components;
		elements[0].offset = attribInfos[0].offset;
		elements[0].type = ConvertType(attribInfos[0].type);

		elements[1].setName("in_normal");
		elements[1].num_components = attribInfos[1].num_components;
		elements[1].offset = attribInfos[1].offset;
		elements[1].type = ConvertType(attribInfos[1].type);

		elements[2].setName("in_tex0");
		elements[2].num_components = attribInfos[2].num_components;
		elements[2].offset = attribInfos[2].offset;
		elements[2].type = ConvertType(attribInfos[2].type);

		elements[0].stream_index = attribInfos[0].buffer_index;
		elements[1].stream_index = attribInfos[1].buffer_index;
		elements[2].stream_index = attribInfos[2].buffer_index;
		
		IInputLayout* input_layout = gapi->createInputLayout(elements, 3);
		gapi->setInputLayout(input_layout);

		// old vertex attribs
		/*
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
		*/


		// set stuff
		gapi->setShaderProgram(shader);
		gapi->setRenderTargets(0, 0);

		mm::mat4 prs =
			mm::create_translation(entity->getPos())
			*mm::mat4(entity->getRot())
			*mm::create_scale(entity->getScale());

		mm::mat4 wvp = scene.getCamera()->getProjMatrix() * scene.getCamera()->getViewMatrix() * prs;

		rBuffer ubo_alloc_data;
			ubo_alloc_data.is_persistent = false;
			ubo_alloc_data.is_readable = true;
			ubo_alloc_data.is_writable = true;
			ubo_alloc_data.prefer_cpu_storage = false;
			ubo_alloc_data.size = 1 * sizeof(mm::mat4);
		auto ubo_buf = gapi->createUniformBuffer(ubo_alloc_data);
		auto index_vs = shader->getUniformBlockIndex("constant_data");

		gapi->writeBuffer(ubo_buf, &wvp, sizeof(mm::mat4), 0);
		gapi->setUniformBuffer(ubo_buf, index_vs);

		// set vertex buffer (old)
		/*/
		IVertexBuffer* tmp[3] = {
			mesh->getVertexBuffers()[attribInfos[0].buffer_index].vb,
			mesh->getVertexBuffers()[attribInfos[1].buffer_index].vb,
			mesh->getVertexBuffers()[attribInfos[2].buffer_index].vb
		};
		gapi->setVertexBuffers(tmp, attribs, 3);
		/*/

		// set vertex buffers (new)
		for (int i = 0; i < mesh->getNumVertexBuffers(); i++) {
			auto stream = mesh->getVertexBuffers()[i];
			gapi->setVertexBuffers(&stream.vb, &stream.stride, &stream.offset, i, 1);
		}
		//*/


		// set index buffer
		auto ib = mesh->getIndexBuffer();
		if (ib)
			gapi->setIndexBuffer(mesh->getIndexBuffer());

		// get material
		Material* mtl = entity->getMaterial();

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

		// draw each mesh mat id group
		auto& matGroups = mesh->getMaterialIds();
		for (auto& matGroup : matGroups) {
			// get corresponding material id
			ITextureGapi* texture = nullptr; // gapi resource

			// if has mtl
			if (mtl && mtl->getNumSubMaterials() > matGroup.id) {
				Material::SubMaterial subMat = mtl->getSubMaterial(matGroup.id);
				ps_const.diffuse = subMat.base;
				if (subMat.t_diffuse) {
					texture = ((Texture*)subMat.t_diffuse)->getTexture();
				}
				ps_const.hasTex = (texture != nullptr);
			}
			else {
				ps_const.diffuse = mm::vec4(1, 0, 0, 1);
				ps_const.hasTex = false;
			}

			// set uniform buffer
			gapi->writeBuffer(ps_const_buf, &ps_const, sizeof(ps_const), 0);
			gapi->setUniformBuffer(ps_const_buf, index_ps);

			// set texture
			if (ps_const.hasTex) {
				gapi->setTexture(texture, shader->getSamplerIndex("tex"));
			}

			// draw matgroup
			gapi->drawIndexed((matGroup.endFace - matGroup.beginFace) * 3, matGroup.beginFace * 3 * sizeof(u32));
		}

		input_layout->release();
		ps_const_buf->destroy();
		ubo_buf->destroy();
	}
}

IGapi* GraphicsEngineRaster::getGapi() {
	return gapi;
}