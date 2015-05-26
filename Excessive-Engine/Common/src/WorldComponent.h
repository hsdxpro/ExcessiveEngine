#pragma once
// Represent those components which can be placed in a 3D world (pos, rot, scale), attachable to Actor, and to itself
#include <vector>
#include "BasicTypes.h"
#include "mymath\mymath.h"
#include "Component.h"

class WorldComponent : public Component
{
public:
	WorldComponent();
	~WorldComponent();

	virtual void updateAfterPhysicsSimulate();

	template<class T>
	T* addChild(T* c)
	{
		static_assert(std::is_base_of<Component, T>::value, "");

		//c->setParent(this);
		childsTypeContainer[c->componentTypeID].push_back(c);
		return c;
	}

	//template<class T>
	//T* setParent(T* c)
	//{
	//	//parent->childsTypeContainer[FAILL__].push_back(c);
	//	//parent = c;
	//	//return c;
	//}

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

		assert(childsTypeContainer[T::componentTypeID].size() > index);
		return static_cast<T*>(childsTypeContainer[T::componentTypeID][index]);
	}

	template<class T>
	u32 getNComp()
	{
		static_assert(std::is_base_of<Component, T>::value, "");

		return childsTypeContainer[T::componentTypeID].size();
	}

protected:
	std::vector<std::vector<WorldComponent*>> childsTypeContainer;
	WorldComponent* parent;

	// World Transform
	mm::vec3 pos;
	mm::vec3 scale;
	mm::quat rot;
};