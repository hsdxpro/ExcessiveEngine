#include "Actor.h"

Actor::Actor()
:worldEntity(0), onUpdate(nullptr), onCollision(nullptr), onCollisionEnter(nullptr), onCollisionExit(nullptr), bPendingKill(false)
{

}
