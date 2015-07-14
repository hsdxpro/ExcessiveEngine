#pragma once
// ActorScript are special scripts, cuz their default ActorBehavior is to operate on 1 actor, but allows N actor too if needed

#include "Actor.h"

class ActorScript
{
public:
	ActorScript(Actor* actor);

public:
	virtual void Update(float deltaTime){}

	__inline void	SetActor(Actor* a) {actor = a;}
	__inline Actor* GetActor() {return actor;}

protected:
	Actor* actor;
};