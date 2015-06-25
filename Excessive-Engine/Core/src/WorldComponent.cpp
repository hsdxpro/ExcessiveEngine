#include "ComponentGraphics.h"
#include "ComponentRigidBody.h"
#include "ComponentSoftBody.h"
#include "ComponentCamera.h"

WorldComponent::WorldComponent()
:parent(0)
{
}

WorldComponent::~WorldComponent()
{
}

void WorldComponent::UpdateAfterPhysicsSimulate()
{
}

WorldComponent* WorldComponent::Attach(WorldComponent* c)
{
	if (c == this)
		return nullptr;

	c->SetParent(this);

	return c;
}

WorldComponent* WorldComponent::AttachTo(WorldComponent* c)
{
	SetParent(c);
	return c;
}

WorldComponent* WorldComponent::Detach()
{
	WorldComponent* oldParent = parent;
	
	SetParent(nullptr);

	return oldParent;
}

WorldComponent* WorldComponent::SetParent(WorldComponent* c)
{
	if (parent == this)
		return nullptr;	

	if (parent)
	{
		auto it = parent->childs.begin();
		while (it != parent->childs.end())
		{
			if (*it == c || !c && *it == this)
			{
				parent->childs.erase(it);
				break;
			}
			it++;
		}
	}

	parent = c;

	if (parent)
	{
		// Recalc relative transform...
		relTransform.SetRot(transform.GetRot() * mm::inverse(parent->transform.GetRot()));
		relTransform.SetScaleLocal(transform.GetScaleLocal() / parent->transform.GetScaleLocal());
		relTransform.SetPos(mm::rotate_vector(mm::inverse(parent->transform.GetRot()), (transform.GetPos() - parent->transform.GetPos()) / parent->transform.GetScaleLocal()));
		relTransform.SetSkew(transform.GetSkew() * mm::inverse(parent->transform.GetSkew()));

		parent->childs.push_back(this);
	}
	else // become root component
	{
		relTransform = transform;
	}
	
	return c;
}

void WorldComponent::SetPos(const mm::vec3& v)
{
	// Delta world space move
	mm::vec3 dMove = v - transform.GetPos();

	// Set position for "this", reflect to component
	transform.SetPos(v);
	_InnerReflectPos();

	// We have parent, update relative transform
	if(parent)
		relTransform.Move(mm::inverse(GetSkew()) * mm::rotate_vector(mm::inverse(GetRot()), dMove));

	// Move childs also
	for(auto& c : childs)
		_InnerMoveChildRecursively(c, dMove);
}

void WorldComponent::SetRot(const mm::quat& q)
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

void WorldComponent::SetScaleLocal(const mm::vec3& v)
{
	mm::vec3 dScale = v / transform.GetScaleLocal();

	transform.SetSkew(mm::create_scale(v));
	_InnerReflectSkew();

	// If we got parent update relative skew...
	if(parent)
		relTransform.SetSkew(transform.GetSkew() * mm::inverse(parent->transform.GetSkew()));

	// dScale - el elõre szorzódik a child world skew....
	//Scale childs
	for (auto& c : childs)
		_InnerScaleChildRecursively(c, transform.GetPos(), transform.GetRot(), transform.GetScaleLocal(), transform.GetSkew(), dScale, mm::inverse(transform.GetRot()));
}

void WorldComponent::SetRelPos(const mm::vec3& v)
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
	for(auto& c : childs)
		_InnerMoveChildRecursively(c, dMove);
}

void WorldComponent::SetRelRot(const mm::quat& q)
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

void WorldComponent::SetRelScale(const mm::vec3& v)
{
	assert(0);
}

void WorldComponent::Move(const mm::vec3& v)
{
	SetPos(GetPos() + v);
}

void WorldComponent::Rot(const mm::quat& q)
{
	SetRot(q * GetRot());
}

void WorldComponent::Scale(const mm::vec3& v)
{
	assert(0);
}

void WorldComponent::ScaleLocal(const mm::vec3& v)
{
	SetScaleLocal(GetScaleLocal() * v);
}

void WorldComponent::MoveRel(const mm::vec3& v)
{
	SetRelPos(GetRelPos() + v);
}

void WorldComponent::RotRel(const mm::quat& q)
{
	SetRelRot(q * GetRelRot());
}

void WorldComponent::ScaleRel(const mm::vec3& v)
{
	assert(0);
}

const Transform3D WorldComponent::GetTransform() const
{
	Transform3D transform;
		transform.SetPos(GetPos());
		transform.SetRot(GetRot());
		//transform.SetScaleLocal(GetScaleLocal());
		transform.SetSkew(GetSkew());
	return transform;
}

void WorldComponent::_InnerMoveChildRecursively(WorldComponent* child, const mm::vec3& dMove)
{
	child->transform.Move(dMove);
	child->_InnerReflectPos();

	for(auto& c : child->childs)
		_InnerMoveChildRecursively(c, dMove);
}

void WorldComponent::_InnerRotChildRecursively(WorldComponent* child, const mm::quat& dRot, const mm::vec3& transformRootPos)
{
	// Rotate child, reflect it to component
	child->transform.Rot(dRot);
	child->_InnerReflectRot();

	// Child rotations causes position change...
	child->transform.SetPos(transformRootPos + mm::rotate_vector(dRot, child->transform.GetPos() - transformRootPos));
	child->_InnerReflectPos();

	// Rot childs
	for (auto& c : child->childs)
		_InnerRotChildRecursively(c, dRot, transformRootPos);
}

void WorldComponent::_InnerScaleChildRecursively(WorldComponent* child, const mm::vec3& parentPos, const mm::quat& parentRot, const mm::vec3& parentLocalScale,  const mm::mat3& parentSkew, const mm::vec3& dScale, const mm::quat& rootRotInverse)
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
		_InnerScaleChildRecursively(c, child->transform.GetPos(), child->transform.GetRot(), child->transform.GetScaleLocal(), child->transform.GetSkew(), dScale, rootRotInverse);
}