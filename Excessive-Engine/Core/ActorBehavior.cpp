#include "ActorBehavior.h"


void ActorBehavior::AddActorScript(ActorScript* s)
{
	actorScripts.push_back(s);
}

void ActorBehavior::SetActor(Actor* a)
{
	for(auto& s : actorScripts)
		s->SetActor(a);
}