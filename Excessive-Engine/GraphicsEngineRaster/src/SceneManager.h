#pragma once

#include <unordered_set>
#include "utility/ref_ptr.h"


////////////////////////////////////////////////////////////////////////////////
// Forward declare
class Entity;
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

	void add(PlaneEntity* entity);
	void add(MeshEntity* entity);
	void add(Light* light);

	void remove(PlaneEntity* entity);
	void remove(MeshEntity* entity);
	void remove(Light* light);

	void clear();
private:

};