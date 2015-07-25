#pragma once
// Behaviors are groups of {actorScript0, actorScript1, ...} which control Actor
#include "EntityScript.h"
#include <vector>

class Behavior
{
public:
	void AddScript(EntityScript* s);

	void SetEntity(Entity* a);

	__inline std::vector<EntityScript*> GetScripts() {return entityScripts;}

protected:
	std::vector<EntityScript*> entityScripts;
};