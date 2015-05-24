#pragma once
#include <vector>
#include "BasicTypes.h"
#include "mymath\mymath.h"
#include "WorldComponent.h"

// The class which has graphical, physical, etc representation in the game world
// That you can transform, iteract with it.
class Actor : public WorldComponent
{
public:
	Actor();
};