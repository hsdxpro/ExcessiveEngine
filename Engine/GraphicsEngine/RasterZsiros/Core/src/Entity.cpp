#include "Entity.h"

cEntity::cEntity(IGraphicsEntity* g, IPhysicsEntity* p)
:graphicsEntity(g), physicsEntity(p) {
}

void cEntity::SetPos(const Vec3& pos) {
	// Set Position for graphics and Physics to
	if(graphicsEntity) graphicsEntity->SetPos(pos);
	if(physicsEntity)  physicsEntity->SetPos(pos);
}

void cEntity::SetRot(const Quat& rot) {
	// Set rotation for graphics and Physics to
	if (graphicsEntity) graphicsEntity->SetRot(rot);
	if (physicsEntity)  physicsEntity->SetRot(rot);
}

void cEntity::SetScale(const Vec3& scale) {
	if (graphicsEntity) graphicsEntity->SetScale(scale);
	if (physicsEntity)  physicsEntity->SetScale(scale);
}

const Vec3& cEntity::GetPos() {
	if(physicsEntity)   return physicsEntity->GetPos();
	if (graphicsEntity) return graphicsEntity->GetPos();
	return Vec3();
}

IGraphicsEntity* cEntity::GetGraphicsEntity() {
	return graphicsEntity;
}

IPhysicsEntity* cEntity::GetPhysicsEntity() {
	return physicsEntity;
}