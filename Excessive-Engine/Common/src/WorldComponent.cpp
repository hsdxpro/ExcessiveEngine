#include "WorldComponent.h"
#include <assert.h>
#include "Sys.h"

WorldComponent::WorldComponent(eCompType type)
:type(type), pos(0, 0, 0), rot(0, mm::vec3(1, 0, 0)), scale(1, 1, 1)
{
	componentTypeContainer.resize(eCompType::COUNT);
}

WorldComponent::~WorldComponent()
{
	// Recursive delete of worldComponents
	for (auto& type : componentTypeContainer)
		for (auto& comp : type)
			delete comp;
}

void WorldComponent::addChild(WorldComponent* comp)
{
	componentTypeContainer[(u8)comp->getType()].push_back(comp);
}

void WorldComponent::setPos(const mm::vec3& v)
{
	pos = v;

	//TODO:REMOVE !! TMP CODE Recursively set position
	for (u32 i = 0; i < eCompType::COUNT; i++)
		for (auto& comp : componentTypeContainer[i])
			comp->setPos(pos);
}

void WorldComponent::setScale(const mm::vec3& v)
{
	scale = v;

	//TODO:REMOVE !! TMP CODE Recursively set position
	for (u32 i = 0; i < eCompType::COUNT; i++)
		for (auto& comp : componentTypeContainer[i])
			comp->setScale(scale); // TODO, JUST CURRENTLY SAFE TO CAST TO WorldComponent, ... check eComType
}

void WorldComponent::setRot(const mm::quat& q)
{
	rot = q;

	//TODO:REMOVE !! TMP CODE Recursively set position
	for (u32 i = 0; i < eCompType::COUNT; i++)
		for (auto& comp : componentTypeContainer[i])
			comp->setRot(rot); 
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

graphics::IEntity* WorldComponent::getCompGraphics(u32 index /*= 0*/)
{
	return (graphics::IEntity*)(componentTypeContainer[eCompType::GRAPHICS][index]);
}

physics::IEntityRigid* WorldComponent::getCompRigid(u32 index /*= 0*/)
{
	return (physics::IEntityRigid*)(componentTypeContainer[eCompType::RIGID][index]);
}

physics::IEntitySoft* WorldComponent::getCompSoft(u32 index /*= 0*/)
{
	return (physics::IEntitySoft*)(componentTypeContainer[eCompType::SOFT][index]);
}

eCompType WorldComponent::getType() const
{
	return type;
}