#pragma once
// Behaviors are groups of {actorScript0, actorScript1, ...} which control Actor
#include "ActorScript.h"
#include <vector>

class Behavior
{
public:
	__inline void AddScript(ActorScript* s)
	{
		entityScripts.push_back(s);
	}

	__inline void SetActor(Actor* a)
	{
		for (auto& s : entityScripts)
			s->SetActor(a);
	}

	__inline std::vector<ActorScript*> GetScripts() {return entityScripts;}

protected:
	std::vector<ActorScript*> entityScripts;
};