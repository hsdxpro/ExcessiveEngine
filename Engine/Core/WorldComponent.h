#pragma once
// Represent those components which can be placed in a 3D world (pos, rot, scale), attachable to Actor, and to itself
#include <vector>
#include "mymath\mymath.h"
#include "mymath\mm_quat_func.h"
#include "SupportLibrary\Transform3D.h"

class WorldComponent
{
public:
	WorldComponent();
	~WorldComponent();

public:
	virtual void UpdateAfterPhysicsSimulate() {}

	__inline void Attach(WorldComponent* c)
	{
		c->AttachTo(this);
	}

	__inline void AttachTo(WorldComponent* c)
	{
		Detach();

		if (parent != c)
		{
			c->childs.push_back(this);
			parent = c;

			relTransform.SetRel(transform, parent->transform);
		}
	}

	__inline WorldComponent* WorldComponent::Detach()
	{
		WorldComponent* savedParent = parent;

		if (parent)
		{
			parent->DetachChild(this);
			relTransform = transform;
		}

		return savedParent;
	}

	__inline bool DetachChild(WorldComponent* c)
	{
		c->parent = nullptr;

		for (size_t i = 0; i < childs.size(); i++)
		{
			if (childs[i] == c)
			{
				// childs[i] not last element, move data forward from behind
				if (i + 1 < childs.size())
					memmove(&childs[i], &childs[i + 1], childs.size() - 1 - i);

				childs.resize(childs.size() - 1);

				// Recalc relative transform
				return true;
			}
		}
		return false;
	}

	__inline void SetParent(WorldComponent* c)
	{
		// Detach old parent potentially
		if (parent)
			parent->Detach();

		AttachTo(c);

		// Recalc relative transform...
		relTransform.SetRot(transform.GetRot() * mm::inverse(parent->transform.GetRot()));
		relTransform.SetScaleLocal(transform.GetScaleLocal() / parent->transform.GetScaleLocal());
		relTransform.SetPos(mm::rotate_vector(mm::inverse(parent->transform.GetRot()), (transform.GetPos() - parent->transform.GetPos()) / parent->transform.GetScaleLocal()));
		relTransform.SetSkew(transform.GetSkew() * mm::inverse(parent->transform.GetSkew()));
	}

	__inline void SetRelTransform(const Transform3D& t) { relTransform = t; }
	__inline void SetTransform(const Transform3D& t) { transform = t; }

	__inline void SetPos(const mm::vec3& v)
	{
		// Delta world space move
		mm::vec3 dMove = v - transform.GetPos();

		// Set position for "this", reflect to component
		transform.SetPos(v);
		_InnerReflectPos();

		// We have parent, update relative transform
		if (parent)
			relTransform.Move(mm::inverse(GetSkew()) * mm::rotate_vector(mm::inverse(GetRot()), dMove));

		// Move childs also
		for (auto& c : childs)
			_InnerMoveChildRecursively(c, dMove);
	}

	__inline void SetRot(const mm::quat& q)
	{
		// Delta world space rot
		mm::quat dRot = q * mm::inverse(transform.GetRot());

		// Set rotattion for "this", reflect to component
		transform.SetRot(q);
		_InnerReflectRot();

		// We have parent, update relative transform
		if (parent)
			relTransform.SetRot(mm::inverse(parent->transform.GetRot()) * transform.GetRot());

		// Rotate childs also, "this" rotation causes childs to move in world space, not just rot
		for (auto& c : childs)
			_InnerRotChildRecursively(c, dRot, transform.GetPos());
	}

	__inline void SetScaleLocal(const mm::vec3& v)
	{
		mm::vec3 dScale = v / transform.GetScaleLocal();

		transform.SetSkew(mm::create_scale(v));
		_InnerReflectSkew();

		// If we got parent update relative skew...
		if (parent)
			relTransform.SetSkew(transform.GetSkew() * mm::inverse(parent->transform.GetSkew()));

		// dScale - el elõre szorzódik a child world skew....
		//Scale childs
		for (auto& c : childs)
			_InnerScaleChildRecursively(c, transform.GetPos(), transform.GetRot(), transform.GetScaleLocal(), transform.GetSkew(), dScale, mm::inverse(transform.GetRot()));
	}

	__inline void SetRelPos(const mm::vec3& v)
	{
		mm::vec3 relDMove = v - relTransform.GetPos();

		// Update relative position
		relTransform.SetPos(v);

		// Calculate the worldSpace delta move to move childs easily, their relative transform cant change
		mm::vec3 dMove = mm::rotate_vector(GetRot(), GetSkew() * relDMove);

		// Relative moving also changes world moving, update "this"
		transform.Move(dMove);
		_InnerReflectPos();

		// dMove is in world Space now, move childs
		for (auto& c : childs)
			_InnerMoveChildRecursively(c, dMove);
	}

	__inline void SetRelRot(const mm::quat& q)
	{
		mm::quat relRot = mm::inverse(relTransform.GetRot()) * q;

		// Update relative rotation
		relTransform.SetRot(q);

		// Calculate the worldSpace delta rot to rot childs easily, their relative transform cant change
		mm::quat dRot = GetRot() * relRot;

		// Relative rotating also changes world rotation..
		transform.Rot(dRot);
		_InnerReflectRot();

		// dRot is in world Space now, rotate childs
		for (auto& c : childs)
			_InnerRotChildRecursively(c, dRot, transform.GetPos());
	}

	__inline void SetRelScale(const mm::vec3& v)
	{
		assert(0);
	}

	__inline void Move(const mm::vec3& v)
	{
		SetPos(GetPos() + v);
	}

	__inline void Rot(const mm::quat& q)
	{
		SetRot(q * GetRot());
	}

	__inline void Scale(const mm::vec3& v)
	{
		assert(0);
	}

	__inline void ScaleLocal(const mm::vec3& v)
	{
		SetScaleLocal(GetScaleLocal() * v);
	}

	__inline void MoveRel(const mm::vec3& v)
	{
		SetRelPos(GetRelPos() + v);
	}

	__inline void RotRel(const mm::quat& q)
	{
		SetRelRot(q * GetRelRot());
	}

	__inline void ScaleRel(const mm::vec3& v)
	{
		assert(0);
	}

	__inline WorldComponent* GetParent() const {return parent;}

	__inline const mm::vec3  GetScaleLocal() const		{return transform.GetScaleLocal();}
	__inline const mm::mat3& GetSkew()		 const		{return transform.GetSkew();}
	__inline const mm::vec3& GetPos()		 const		{return transform.GetPos();}
	__inline const mm::quat& GetRot()		 const		{return transform.GetRot();}

	__inline const mm::vec3  GetRelScaleLocal() const	{return relTransform.GetScaleLocal();}
	__inline const mm::vec3& GetRelPos()		const	{return relTransform.GetPos();}
	__inline const mm::quat& GetRelRot()		const	{return relTransform.GetRot();}
	

	__inline const Transform3D& GetRelTransform()	const {return relTransform;}
	__inline const Transform3D& GetTransform()		const {return transform;}

	__inline mm::vec3 GetFrontDirNormed()	const {return mm::rotate_vector(GetRot(), mm::vec3( 0,  1,  0));}
	__inline mm::vec3 GetBackDirNormed()	const {return mm::rotate_vector(GetRot(), mm::vec3( 0, -1,  0));}
	__inline mm::vec3 GetUpDirNormed()		const {return mm::rotate_vector(GetRot(), mm::vec3( 0,  0,  1));}
	__inline mm::vec3 GetDownDirNormed()	const {return mm::rotate_vector(GetRot(), mm::vec3( 0,  0, -1));}
	__inline mm::vec3 GetRightDirNormed()	const {return mm::rotate_vector(GetRot(), mm::vec3( 1,  0,  0));}
	__inline mm::vec3 GetLeftDirNormed()	const {return mm::rotate_vector(GetRot(), mm::vec3(-1,  0,  0));}

	__inline std::vector<WorldComponent*>& GetChilds() { return childs; }

protected:
	virtual void _InnerReflectPos(){}
	virtual void _InnerReflectRot(){}
	virtual void _InnerReflectSkew(){}

	__inline void _InnerMoveChildRecursively(WorldComponent* child, const mm::vec3& dMove)
	{
		child->transform.Move(dMove);
		child->_InnerReflectPos();

		for (auto& c : child->childs)
			child->_InnerMoveChildRecursively(c, dMove);
	}

	__inline void _InnerRotChildRecursively(WorldComponent* child, const mm::quat& dRot, const mm::vec3& transformRootPos)
	{
		// Rotate child, reflect it to component
		child->transform.SetRot(transform.GetRot() * child->GetRelRot());
		child->_InnerReflectRot();

		// Child rotations causes position change...
		child->transform.SetPos(transformRootPos + mm::rotate_vector(dRot, child->transform.GetPos() - transformRootPos));
		child->_InnerReflectPos();

		// Rot childs
		for (auto& c : child->childs)
			child->_InnerRotChildRecursively(c, dRot, transformRootPos);
	}

	__inline void _InnerScaleChildRecursively(WorldComponent* child, const mm::vec3& parentPos, const mm::quat& parentRot, const mm::vec3& parentLocalScale, const mm::mat3& parentSkew, const mm::vec3& dScale, const mm::quat& rootRotInverse)
	{
		// Set child new skew
		mm::mat4 rotRelToRoot = mm::mat4(rootRotInverse * child->transform.GetRot());
		mm::mat4 invRotRelToRoot = mm::mat4(mm::inverse(rootRotInverse * child->transform.GetRot()));

		child->transform.SetSkew(mm::mat4(child->transform.GetSkew()) * invRotRelToRoot * mm::create_scale(dScale) * rotRelToRoot);
		child->_InnerReflectSkew();

		// Set child new position
		child->transform.SetPos(parentPos + mm::rotate_vector(parentRot, child->relTransform.GetPos() * parentLocalScale));
		child->_InnerReflectPos();

		// Skew childs
		for (auto& c : child->childs)
			child->_InnerScaleChildRecursively(c, child->transform.GetPos(), child->transform.GetRot(), child->transform.GetScaleLocal(), child->transform.GetSkew(), dScale, rootRotInverse);
	}

protected:
	// Child components in the tree structure
	std::vector<WorldComponent*> childs;

	// Parent component
	WorldComponent* parent;

	// World and Relative transformation
	Transform3D transform;
	Transform3D relTransform;
};