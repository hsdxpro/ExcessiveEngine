#pragma once

class Actor;
class ActorScript
{
public:
	ActorScript();

public:
	virtual void Update(float deltaTime){}

	__inline void SetActor(Actor* a) { actor = a; }

	__inline Actor* GetActor() {return actor;}

protected:
	Actor* actor;
};