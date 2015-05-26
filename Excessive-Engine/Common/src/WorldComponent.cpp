#include "ComponentGraphics.h"
#include "ComponentRigidBody.h"
#include "ComponentSoftBody.h"

// Update this when u add new component type !
enum eTypeID
{
	_0,
	_1,
	_2,

	COUNT,
};

// Update this when u add new component type !
u8 ComponentGraphics::componentTypeID = eTypeID::_0;
u8 ComponentRigidBody::componentTypeID = eTypeID::_1;
u8 ComponentSoftBody::componentTypeID = eTypeID::_2;


WorldComponent::WorldComponent()
:parent(0), scale(1,1,1)
{
	childsTypeContainer.resize(eTypeID::COUNT);
}

WorldComponent::~WorldComponent()
{
	// Recursively delete hierarchy
	for (auto compTypes : childsTypeContainer)
		for (auto child : compTypes)
			delete child;
}

void WorldComponent::updateAfterPhysicsSimulate()
{

}

void WorldComponent::setPos(const mm::vec3& v)
{
	pos = v;

	for (auto& compTypes : childsTypeContainer)
		for (auto& comp : compTypes)
			comp->setPos(v);
}

void WorldComponent::setRot(const mm::quat& q)
{
	rot = q;

	for (auto& compTypes : childsTypeContainer)
		for (auto& comp : compTypes)
			comp->setRot(q);
}

void WorldComponent::setScale(const mm::vec3& v)
{
	scale = v;

	for (auto& compTypes : childsTypeContainer)
		for (auto& comp : compTypes)
			comp->setScale(v);
}

const mm::vec3& WorldComponent::getPos()
{
	return pos;
}

const mm::quat& WorldComponent::getRot()
{
	return rot;
}

const mm::vec3& WorldComponent::getScale()
{
	return scale;
}