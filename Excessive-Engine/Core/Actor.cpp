#include "Actor.h"

Actor::Actor()
:entity(0), onUpdate(nullptr), onCollision(nullptr), onCollisionEnter(nullptr), onCollisionExit(nullptr), bPendingKill(false)
{

}
