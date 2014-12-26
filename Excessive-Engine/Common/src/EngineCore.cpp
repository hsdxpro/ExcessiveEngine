#include "EngineCore.h"
#include "Factory.h"
#include "Importer3D.h"

EngineCore::EngineCore() 
:graphicsEngine(0) {
}

EngineCore::~EngineCore() {
	for (auto& a : entities)
		delete a;
}

graphics::IGraphicsEngine* EngineCore::initGraphicsEngineRaster(const rGraphicsEngineRaster& desc) {
	if (graphicsEngine) 
		graphicsEngine->release(); 

	return graphicsEngine = Factory::createGraphicsEngineRaster(desc);
}

// Init graphics engine, if one already exists will be destroyed, then instantiate it
graphics::IGraphicsEngine* EngineCore::initGraphicsEngineRT(const rGraphicsEngineRT& desc) {
	if (graphicsEngine) 
		graphicsEngine->release(); 
	
	return Factory::createGraphicsEngineRT(desc);
}

Entity* EngineCore::createEntity(graphics::IScene* gScene, const std::wstring& modelPath)
{
	// Config for importing
	rImporter3DCfg cfg({ eImporter3DFlag::VERT_INTERLEAVED, 
						 eImporter3DFlag::VERT_ATTR_POS, 
						 eImporter3DFlag::VERT_ATTR_NORM, 
						 eImporter3DFlag::VERT_ATTR_TEX0 });
	rImporter3DData desc;
	Importer3D::loadFile(modelPath, cfg, desc);

	// We will feed meshes to that graphics entity
	graphics::IEntity* gEntity = gScene->createEntity();

	// For each mesh imported, create graphics mesh
	for (auto& a : desc.meshes) {
		graphics::IMesh* mesh = graphicsEngine->createMesh();

		std::vector<u32> matIDs = { 0 };
		matIDs.reserve(a.materials.size());
		for(auto& m : a.materials) {
			matIDs.push_back(m.faceStartIdx);
		}
		
		graphics::IMesh::MeshData meshData;
			meshData.index_data = a.indices.data();
			meshData.index_num = a.indices.size();
			meshData.mat_ids = matIDs.data();
			meshData.mat_ids_num = matIDs.size();
			meshData.vertex_bytes = a.nVertices * a.vertexSize;
			meshData.vertex_data = a.vertexBuffers[0];

			graphics::IMesh::ElementDesc elements[] = {
				graphics::IMesh::POSITION, 3,
				graphics::IMesh::NORMAL, 3,
				graphics::IMesh::TEX0, 2,
			};
			meshData.vertex_elements = elements;
			meshData.vertex_elements_num = sizeof(elements) / sizeof(elements[0]);

		// Feed data to mesh
		mesh->update(meshData);

		// Set entity's mesh
		// TODO: need add not set
		assert(desc.meshes.size() <= 1);

		gEntity->setMesh(mesh);
	}

	// new entity created
	Entity* e = new Entity(gEntity);
		entities.push_back(e);
	return e;
}
