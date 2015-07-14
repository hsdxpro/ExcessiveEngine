#pragma once
// Behaviors are groups of {actorScript0, actorScript1, ...} which control Actor
#include "ActorScript.h"
#include <vector>

class ActorBehavior
{
public:
	void AddActorScript(ActorScript* s);

	void SetActor(Actor* a);

	__inline std::vector<ActorScript*> GetActorScripts() {return actorScripts;}

protected:
	std::vector<ActorScript*> actorScripts;
};