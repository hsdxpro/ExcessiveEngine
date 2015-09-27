#pragma once
#include "PhysicsEngine\PhysicsCommon.h"
#include "SoftBodyComponent.h"
#include "RigidBodyComponent.h"
#include "SoundEngine\ISoundData.h"
#include "SoundEngine\IEmitter.h"
#include <functional>

struct rMonoSound
{
	sound::ISoundData* soundData;
	sound::IEmitter* soundEmitter;
};

struct rTask
{
	std::function<void()> callb;
	float timeLeft;
};

struct rCollide
{
	// self RigidBody colliding, otherwise nullptr
	RigidBodyComponent* rigidBodyA;

	// other RigidBody colliding, otherwise nullptr
	RigidBodyComponent* rigidBodyB;

	// self SoftBody colliding, otherwise nullptr
	SoftBodyComponent* softBodyA;

	// other SoftBody colliding, otherwise nullptr
	SoftBodyComponent* softBodyB;

	// Contact points generated between bodyA and bodyB
	std::vector<physics::rContactPoint> contacts;
};

class Actor;
struct rCollision
{
	Actor*  actorA;
	Actor*  actorB;

	// All collisions between actorA and actorB in that frame, it contains "Current bodyA bodyB collision" too
	std::vector<rCollide> collisions;

// Current bodyA bodyB collision
	// self RigidBody colliding, otherwise nullptr
	RigidBodyComponent* rigidBodyA;

	// other RigidBody colliding, otherwise nullptr
	RigidBodyComponent* rigidBodyB;
	
	// self SoftBody colliding, otherwise nullptr
	SoftBodyComponent* softBodyA;

	// other SoftBody colliding, otherwise nullptr
	SoftBodyComponent* softBodyB;
	
	// Contact points generated between bodyA and bodyB
	std::vector<physics::rContactPoint> contacts;
};

struct rPhysicsTraceResult
{
	Actor* actor;

	mm::vec3 pos;
	mm::vec3 normal;
};

struct rPhysicsTraceParams : public physics::rTraceParams
{
};