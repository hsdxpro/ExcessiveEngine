#include "Actor.h"

Actor::Actor(WorldComponent* rootComp)
:rootComp(rootComp), parent(0), onUpdate(nullptr), onCollision(nullptr), onCollisionEnter(nullptr), onCollisionExit(nullptr), bKilled(false)
{
}
