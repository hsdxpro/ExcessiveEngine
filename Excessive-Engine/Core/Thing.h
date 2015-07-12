#pragma once
// Thing is the highest level logical representation of "game objects"
// - Behaviors are groups of {actorScript0, actorScript1, ...} which control Actor
// - Actor is the controlled component tree by Behaviors
#include "ActorBehavior.h"

class Thing
{
public:
	Thing();

public:
	__inline void AddBehavior(ActorBehavior* b)	{behaviors.push_back(b);}

	__inline void SetActor(Actor* a)		{actor = a; for(auto& b : behaviors)b->SetActor(a);}

	__inline Actor* GetActor(Actor* a) const		{return actor;}
	__inline std::vector<ActorBehavior*>& GetBehaviors() {return behaviors;}

protected:
	Actor* actor;

	std::vector<ActorBehavior*> behaviors;
};