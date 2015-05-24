#pragma once

#include <vector>
#include "BasicTypes.h"
#include "mymath\mymath.h"

namespace graphics { class IEntity; }
namespace physics { class IEntityRigid; class IEntitySoft; }

enum eCompType : u8
{
	EMPTY = 0,
	GRAPHICS = 1,
	RIGID = 2,
	SOFT = 3,

	COUNT =4,
};

class WorldComponent
{
public:
	WorldComponent(eCompType type);
	~WorldComponent();

	void addChild(WorldComponent* comp);

	virtual void setPos(const mm::vec3& v);
	virtual void setRot(const mm::quat& v);
	virtual void setScale(const mm::vec3& q);

	virtual const mm::vec3& getPos();
	virtual const mm::quat& getRot();
	virtual const mm::vec3& getScale();

	eCompType getType() const;

	graphics::IEntity*		getCompGraphics(u32 index = 0);
	physics::IEntityRigid*	getCompRigid(u32 index = 0);
	physics::IEntitySoft*	getCompSoft(u32 index = 0);

protected:
	std::vector<std::vector<WorldComponent*>> componentTypeContainer; // components[0] type of rigid components, components[1] type of .....
	eCompType type; // Type of component

	// World Transforms
	mm::vec3 pos;
	mm::vec3 scale;
	mm::quat rot;
};