#include "Entity.h"

void CollectComponentsRecursively(WorldComponent* c, std::vector<WorldComponent*>& comps)
{
	comps.push_back(c);

	for (auto& child : c->GetChilds())
		CollectComponentsRecursively(child, comps);
}

Entity::Entity()
:rootComp(0)
{

}