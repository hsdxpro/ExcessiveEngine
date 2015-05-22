#pragma once

#include "../GraphicsEngine/Interfaces/IEntity.h"
#include "../GraphicsEngine/Interfaces/IMesh.h"
#include "../GraphicsEngine/Interfaces/IEngine.h"
#include "../Common/src/Animator.h"
#include "../Common/src/KeyframeAnimation.h"

bool CreateAnimatedHand(graphics::IEngine* engine, graphics::IMesh*& mesh, KeyframeAnimation& animation);