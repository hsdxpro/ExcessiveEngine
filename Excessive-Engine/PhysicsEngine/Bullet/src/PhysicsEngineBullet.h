#pragma once
#include "../../Interfaces/IEngine.h"

struct rPhysicsEngineBullet {

};

class PhysicsEngineBullet : public physics::IEngine
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

