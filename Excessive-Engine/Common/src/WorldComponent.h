#pragma once

#include <vector>
#include "BasicTypes.h"
#include "mymath\mymath.h"

class Transform
{
public:
	Transform();

	virtual void setPos(const mm::vec3& v);
	virtual void setRot(const mm::quat& q);
	virtual void setScale(const mm::vec3& v);

	virtual const mm::vec3& getPos();
	virtual const mm::quat& getRot();
	virtual const mm::vec3& getScale();

protected:
	// World Transforms
	mm::vec3 pos;
	mm::vec3 scale;
	mm::quat rot;
};

class Component
{

};

class WorldComponent : public Transform, public Component
{
public:
	WorldComponent();

	virtual void updateAfterPhysicsSimulate();

	template<class T>
	void addChild(T* c)
	{
		//static_assert(std::is_base_of<Component, T>::value, "");

		componentTypeContainer[c->componentTypeID].push_back(c);
	}

	virtual void setPos(const mm::vec3& v);
	virtual void setRot(const mm::quat& q);
	virtual void setScale(const mm::vec3& v);

	virtual const mm::vec3& getPos();
	virtual const mm::quat& getRot();
	virtual const mm::vec3& getScale();

	template<class T>
	T*	getComp(u32 index = 0)
	{
		static_assert(std::is_base_of<Component, T>::value, "");

		assert(componentTypeContainer[T::componentTypeID].size() > index);
		return static_cast<T*>(componentTypeContainer[T::componentTypeID][index]);
	}

	template<class T>
	u32 getNComp()
	{
		static_assert(std::is_base_of<Component, T>::value, "");

		return componentTypeContainer[T::componentTypeID].size();
	}

protected:
	std::vector<std::vector<WorldComponent*>> componentTypeContainer;
};

namespace physics { class IEntitySoft; class IEntityRigid; }
namespace graphics{ class IEntity;  }

class ComponentSoftBody : public WorldComponent
{
public:
	static u8 componentTypeID;
	ComponentSoftBody(physics::IEntitySoft* e) :entitySoft(e){}

protected:
	physics::IEntitySoft* entitySoft;
};

class ComponentRigidBody : public WorldComponent
{
public:
	static u8 componentTypeID;
	ComponentRigidBody(physics::IEntityRigid* e):entityRigid(e){}

	void updateAfterPhysicsSimulate(); // After physics simulation done, we transforms all WorldComponent child also

	void setPos(const mm::vec3& v);
	void setRot(const mm::quat& q);
	void setScale(const mm::vec3& v);

	const mm::vec3& getPos();
	const mm::quat& getRot();
	const mm::vec3& getScale();

protected:
	physics::IEntityRigid* entityRigid;
};

class ComponentGraphics : public WorldComponent
{
public:
	static u8 componentTypeID;
	ComponentGraphics(graphics::IEntity* e):entityGraphics(e){}

	void setPos(const mm::vec3& v);
	void setRot(const mm::quat& q);
	void setScale(const mm::vec3& v);

	const mm::vec3& getPos();
	const mm::quat& getRot();
	const mm::vec3& getScale();

protected:
	graphics::IEntity* entityGraphics;
};