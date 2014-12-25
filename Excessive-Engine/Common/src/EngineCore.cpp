#include "EngineCore.h"
#include "Factory.h"

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

Entity* EngineCore::createEntity(graphics::IScene* gScene, const std::wstring& geomPath)
{
	// Itt most annak kell történnie, hogy geomPath cuccost azt felparseoljuk úgy, hogy
	// legyen egy csomó vertexünk, indexünk, ja és csomó material

	// Create simple test mesh that we will assign to graphics entity
	graphics::IMesh* mesh = graphicsEngine->createMesh();
	
	if (!Factory::createResourceLoader()->loadMesh(mesh, geomPath.c_str()))
		return nullptr;
	
	// Assign mesh to entity
	graphics::IEntity* gEntity = gScene->createEntity();
	gEntity->setMesh(mesh);
	
	Entity* e = new Entity(gEntity);
		entities.push_back(e);
	return e;
}
