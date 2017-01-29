// Entity.h By Zsíroskenyér Team 2013.10.25 23:08
// consist of cGraphicsEntity, cPhysicsEntity etc..
#pragma once

#include "IGraphicsEntity.h"
#include "IPhysicsEntity.h"

class cEntity {
public:
	cEntity(IGraphicsEntity* g, IPhysicsEntity* p);

	void SetPos(const Vec3& pos);
	void SetRot(const Quat& rot);
	void SetScale(const Vec3& scale);

	const Vec3& GetPos();

	IGraphicsEntity* GetGraphicsEntity();
	IPhysicsEntity* GetPhysicsEntity();

protected:
	IGraphicsEntity* graphicsEntity;
	IPhysicsEntity* physicsEntity;
};