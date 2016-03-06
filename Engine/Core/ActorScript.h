#pragma once

class Actor;
class ActorScript
{
public:
	ActorScript();

public:
	virtual void Update(float deltaTime){}

	inline void SetActor(Actor* a) { actor = a; }

	inline Actor* GetActor() {return actor;}

protected:
	Actor* actor;
};