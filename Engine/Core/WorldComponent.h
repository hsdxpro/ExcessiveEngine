// Represent those components which can be placed in a 3D world (pos, rot, scale), attachable to Actor, and to itself
#pragma once

#include <vector>
#include "mymath\mymath.h"
#include "mymath\mm_quat_func.h"
#include "SupportLibrary\Transform3D.h"

#include <functional>

class CameraComponent;
class RigidBodyComponent;
class MeshComponent;

enum eWorldComponentType
{
	CAMERA = 1 << 0,
	RIGID_BODY = 1 << 1,
	SOFT_BODY = 1 << 2,
	MESH = 1 << 3,
	TRANSFORM = 1 << 4,
	ALL = 0x1F,
};

class WorldComponent
{
public:
	inline WorldComponent(eWorldComponentType type);
	inline ~WorldComponent();

public:
	CameraComponent*	AddComponent_Camera();
	MeshComponent*		AddComponent_Mesh(const std::string& modelPath);

	inline void Attach(WorldComponent* c)
	{
		c->AttachTo(this);
	}

	inline void AttachTo(WorldComponent* c)
	{
		Detach();

		if (parent != c)
		{
			c->childs.push_back(this);
			parent = c;

			relTransform = Transform3D(parent->transform, transform);
		}
	}

	inline WorldComponent* WorldComponent::Detach()
	{
		WorldComponent* savedParent = parent;

		if (parent)
		{
			parent->DetachChild(this);
			relTransform = transform;
		}

		return savedParent;
	}

	inline bool DetachChild(WorldComponent* c)
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

	inline void SetParent(WorldComponent* c)
	{
		// Detach old parent potentially
		if (parent)
			parent->Detach();

		AttachTo(c);

		// Recalc relative transform...
		relTransform.SetRot(transform.GetRot() * mm::inverse(parent->transform.GetRot()));
		relTransform.SetScale(transform.GetScale() / parent->transform.GetScale());
		relTransform.SetPos(mm::rotate_vector(mm::inverse(parent->transform.GetRot()), (transform.GetPos() - parent->transform.GetPos()) / parent->transform.GetScale()));
		relTransform.SetSkew(transform.GetSkew() * mm::inverse(parent->transform.GetSkew()));
	}

	inline void SetRelTransform(const Transform3D& t)
	{
		relTransform = t;

		// We have parent, update transform
		if (parent)
			bDirtyTransform = true;
		else
			transform = relTransform;
	}

	inline void SetTransform(const Transform3D& t)
	{
		transform = t;

		// Recalculate relative transform when user call "Get..."
		if (parent)
		{
			bDirtyRelativeTransform = true;
		}
		else // Don't have parent, relative transform equals to world transform
		{
			relTransform = transform;
		}
	}

	inline void SetPos(const mm::vec3& v)
	{
		// Recalculate relative transform when user call "Get..."
		if (parent)
		{
			bDirtyRelativeTransform = true;
		}
		else // Don't have parent, relative transform equals to world transform
		{
			relTransform = transform;
		}

		// Delta world space move
		mm::vec3 dMove = v - transform.GetPos();

		// Set world position
		transform.SetPos(v);

		// Move childs also
		for (WorldComponent* c : childs)
			_MoveChild(c, dMove);
	}

	inline void SetRot(const mm::quat& q)
	{
		// Delta world space rot
		mm::quat dRot = q * mm::inverse(transform.GetRot());

		// Set rotation for "this", reflect to component
		transform.SetRot(q);

		// We have parent, update relative transform
		if (parent)
			bDirtyRelativeTransform = true;
		else
			relTransform = transform;

		// Rotate childs also, "this" rotation causes childs to move in world space, not just rot
		for (WorldComponent* c : childs)
			_RotChild(c, dRot, transform.GetPos());
	}

	inline void SetRot(const mm::quat& q, const mm::vec3& rotOrigin)
	{
		//// Delta world space rot
		mm::quat dRot = q * mm::inverse(transform.GetRot());

		transform.SetRot(q, rotOrigin);

		// We have parent, update relative transform
		if (parent)
			bDirtyRelativeTransform = true;
		else
			relTransform = transform;
		
		// Rotate childs also, "this" rotation causes childs to move in world space, not just rot
		for (WorldComponent* c : childs)
			_RotChild(c, dRot, transform.GetPos());
	}

	inline void SetScale(const mm::vec3& v)
	{
		mm::vec3 dScale = v / transform.GetScale();

		transform.SetSkew(mm::create_scale(v));
		//_InnerReflectSkew();

		// If we got parent update relative skew...
		if (parent)
			bDirtyRelativeTransform = true;
		else
			relTransform = transform;

		// dScale - el elõre szorzódik a child world skew....
		//Scale childs
		for (WorldComponent* c : childs)
			_ScaleChild(c, transform.GetPos(), transform.GetRot(), transform.GetScale(), transform.GetSkew(), dScale, mm::inverse(transform.GetRot()));
	}

	inline void SetScale(const mm::vec3& scale, const mm::vec3& rootPos, const mm::quat& rootRot);
	inline void Scale(const mm::vec3& scale, const mm::vec3& rootPos, const mm::quat& rootRot);

	inline void SetRelPos(const mm::vec3& relPos)
	{
		mm::vec3 relDMove = relPos - relTransform.GetPos();

		// Calculate the worldSpace delta move to move childs easily, their relative transform cant change
		mm::vec3 dMove = mm::rotate_vector(GetRot(), GetSkew() * relDMove);

		// Relative moving also changes world moving, update "this"
		transform.Move(dMove);

		if (parent)
			bDirtyRelativeTransform = true;
		else
			relTransform = transform;

		// dMove is in world Space now, move childs
		for (WorldComponent* c : childs)
			_MoveChild(c, dMove);
	}

	inline void SetRelRot(const mm::quat& q)
	{
		mm::quat relRot = mm::inverse(relTransform.GetRot()) * q;

		// Update relative rotation
		relTransform.SetRot(q);

		// Calculate the worldSpace delta rot to rot childs easily, their relative transform cant change
		mm::quat dRot = GetRot() * relRot;

		// Relative rotating also changes world rotation..
		transform.Rot(dRot);
		//_InnerReflectRot();

		// dRot is in world Space now, rotate childs
		for (WorldComponent* c : childs)
			_RotChild(c, dRot, transform.GetPos());
	}

	inline void SetRelScale(const mm::vec3& v)
	{
		assert(0);
	}

	inline void RotX(float angleDegree)
	{
		Rot(mm::quat(mm::radians(angleDegree), mm::vec3(1, 0, 0)));
	}

	inline void RotY(float angleDegree)
	{
		Rot(mm::quat(mm::radians(angleDegree), mm::vec3(0, 1, 0)));
	}

	inline void RotZ(float angleDegree)
	{
		Rot(mm::quat(mm::radians(angleDegree), mm::vec3(0, 0, 1)));
	}

	inline void Move(const mm::vec3& v)
	{
		SetPos(GetPos() + v);
	}

	inline void Rot(const mm::quat& q)
	{
		SetRot(q * GetRot());
	}

	inline void Rot(const mm::quat& q, const mm::vec3& rotOrigin)
	{
		SetRot(q * GetRot(), rotOrigin);
	}

	inline void Scale(const mm::vec3& v)
	{
		// TODO NOT LOCAL SCLAE !!!!!
		SetScale(GetScale() * v);
	}

	inline void ScaleLocal(const mm::vec3& v)
	{
		SetScale(GetScale() * v);
	}

	inline void MoveRel(const mm::vec3& v)
	{
		SetRelPos(GetRelPos() + v);
	}

	inline void RotRel(const mm::quat& q)
	{
		SetRelRot(q * GetRelRot());
	}

	inline void ScaleRel(const mm::vec3& v)
	{
		assert(0);
	}

	inline WorldComponent* GetParent() const {return parent;}

	inline const mm::vec3  GetScale()	const {return transform.GetScale();}
	inline const mm::mat3& GetSkew()		const {return transform.GetSkew();}
	inline const mm::vec3& GetPos()			const {return transform.GetPos();}
	inline const mm::quat& GetRot()			const {return transform.GetRot();}

	inline const mm::vec3  GetRelLocalScale()	const {return relTransform.GetScale();}
	inline const mm::vec3& GetRelPos()			const {return relTransform.GetPos();}
	inline const mm::quat& GetRelRot()			const {return relTransform.GetRot();}
	

	inline const Transform3D& GetRelTransform()	const {return relTransform;}
	inline const Transform3D& GetTransform()	const {return transform;}

	inline mm::vec3 GetFrontDir()	const {return GetTransform().GetFrontDir(); }
	inline mm::vec3 GetBackDir()	const {return GetTransform().GetBackDir();}
	inline mm::vec3 GetUpDir()		const {return GetTransform().GetUpDir();}
	inline mm::vec3 GetDownDir()	const {return GetTransform().GetDownDir();}
	inline mm::vec3 GetRightDir()	const {return GetTransform().GetRightDir();}
	inline mm::vec3 GetLeftDir()	const {return GetTransform().GetLeftDir();}

	inline std::vector<WorldComponent*>& GetChilds() {return childs; }

	inline eWorldComponentType GetType();

	// TODO
	template<class T>
	inline bool Is()
	{
		return type == T::TYPE;
	}

	bool IsRigidBody() const { return type == RIGID_BODY; }
	bool IsMesh() const { return type == MESH; }
	bool IsCamera() const { return type == CAMERA; }

	RigidBodyComponent* AsRigidBody() { assert(IsRigidBody()); return (RigidBodyComponent*)this; }
	CameraComponent* AsCamera() { assert(IsCamera()); return (CameraComponent*)this; }

protected:
	inline void _MoveChild(WorldComponent* child, const mm::vec3& dMove)
	{
		child->transform.Move(dMove);

		for (auto& c : child->childs)
			child->_MoveChild(c, dMove);
	}

	inline void _RotChild(WorldComponent* child, const mm::quat& dRot, const mm::vec3& transformRootPos)
	{
		child->transform.Rot(dRot, transformRootPos);

		// Rot childs
		for (auto& c : child->childs)
			child->_RotChild(c, dRot, transformRootPos);
	}

	inline void _ScaleChild(WorldComponent* child, const mm::vec3& parentPos, const mm::quat& parentRot, const mm::vec3& parentLocalScale, const mm::mat3& parentSkew, const mm::vec3& dScale, const mm::quat& rootRotInverse)
	{
		// Set child new skew
		mm::mat4 rotRelToRoot = mm::mat4(rootRotInverse * child->transform.GetRot());
		mm::mat4 invRotRelToRoot = mm::mat4(mm::inverse(rootRotInverse * child->transform.GetRot()));

		child->transform.SetSkew(mm::mat4(child->transform.GetSkew()) * invRotRelToRoot * mm::create_scale(dScale) * rotRelToRoot);
		//child->_InnerReflectSkew();

		// Set child new position
		child->transform.SetPos(parentPos + mm::rotate_vector(parentRot, child->relTransform.GetPos() * parentLocalScale));
		//child->_InnerReflectPos();

		// Skew childs
		for (auto& c : child->childs)
			child->_ScaleChild(c, child->transform.GetPos(), child->transform.GetRot(), child->transform.GetScale(), child->transform.GetSkew(), dScale, rootRotInverse);
	}

protected:
	eWorldComponentType type;

	// Child components in the tree structure
	std::vector<WorldComponent*> childs;

	// Parent component
	WorldComponent* parent;

	// World and Relative transformation
	Transform3D transform;
	Transform3D relTransform;

	// Dirty flags for transforms
	uint8_t bDirtyTransform : 1;
	uint8_t bDirtyRelativeTransform : 1;
};






WorldComponent::WorldComponent(eWorldComponentType type)
:parent(0), bDirtyRelativeTransform(false), bDirtyTransform(false), type(type)
{
}

WorldComponent::~WorldComponent()
{

}

void WorldComponent::SetScale(const mm::vec3& scale, const mm::vec3& rootPos, const mm::quat& rootRot)
{
	mm::vec3 dScale = scale / transform.GetScale();

	Scale(dScale, rootPos, rootRot);
}

void WorldComponent::Scale(const mm::vec3& scale, const mm::vec3& rootPos, const mm::quat& rootRot)
{
	std::function<void(WorldComponent* child)> func;
	func = [&](WorldComponent* child)
	{
		child->transform.Scale(scale, rootPos, rootRot);

		for (WorldComponent* c : child->childs)
			func(c);
	};

	// Scale hierarchy
	func(this);
}

eWorldComponentType WorldComponent::GetType()
{
	return type;
}