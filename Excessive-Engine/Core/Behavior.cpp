#include "Behavior.h"


void Behavior::AddScript(EntityScript* s)
{
	entityScripts.push_back(s);
}

void Behavior::SetEntity(Entity* a)
{
	for(auto& s : entityScripts)
		s->SetEntity(a);
}