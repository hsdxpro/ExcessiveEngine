#pragma once
// EntityScript are special scripts, cuz their default ActorBehavior is to operate on 1 actor, but allows N actor too if needed

#include "Entity.h"

class EntityScript
{
public:
	EntityScript();

public:
	virtual void Update(float deltaTime){}

	__inline void SetEntity(Entity* e) {entity = e;}

	__inline Entity* GetEntity() {return entity;}

protected:
	Entity* entity;
};