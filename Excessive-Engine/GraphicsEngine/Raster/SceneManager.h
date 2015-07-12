#pragma once

#include <unordered_set>
#include "utility/ref_ptr.h"


////////////////////////////////////////////////////////////////////////////////
// Forward declare
//class Entity;
class PlaneEntity;
class MeshEntity;
class Light;


////////////////////////////////////////////////////////////////////////////////
/// SceneManager stores the whole scene.
/// Manages, sorts, stores entities, lights and other scene objects. Provides
/// facilities for spatial sorting, instancing and others.
////////////////////////////////////////////////////////////////////////////////

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Add(PlaneEntity* entity);
	void Add(MeshEntity* entity);
	void Add(Light* light);

	void Remove(PlaneEntity* entity);
	void Remove(MeshEntity* entity);
	void Remove(Light* light);

	void clear();
private:

};