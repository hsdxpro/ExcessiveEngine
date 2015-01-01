#pragma once
#include "../Module_Interfaces/PhysicsEngine/IPhysicsEngine.h"

struct rPhysicsEngineBullet {

};

class PhysicsEngineBullet : public physics::IPhysicsEngine
{
public:
	// ctor, dtor, release
	PhysicsEngineBullet(const rPhysicsEngineBullet& d);
	~PhysicsEngineBullet();

	void release() override;

	// interact
	void update(float deltaTime) override;

private:
};

