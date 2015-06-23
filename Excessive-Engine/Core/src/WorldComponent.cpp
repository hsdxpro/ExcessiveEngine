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
		relTransform.SetRot(mm::inverse(parent->worldTransform.GetRot()) * worldTransform.GetRot());
		relTransform.SetScaleLocal(worldTransform.GetScaleLocal() / parent->worldTransform.GetScaleLocal());
		relTransform.SetPos(mm::rotate_vector(mm::inverse(parent->worldTransform.GetRot()), (worldTransform.GetPos() - parent->worldTransform.GetPos()) / parent->worldTransform.GetScaleLocal()));
		relTransform.SetSkew(worldTransform.GetSkew() * mm::inverse(parent->worldTransform.GetSkew()));

		parent->childs.push_back(this);
	}
	else // become root component
	{
		relTransform = worldTransform;
	}
	
	return c;
}

void WorldComponent::SetPos(const mm::vec3& v)
{
	// Delta world space move
	mm::vec3 dMove = v - worldTransform.GetPos();

	// Set position for "this", reflect to component
	worldTransform.SetPos(v);
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
	mm::quat dRot = q * mm::inverse(worldTransform.GetRot());

	// Set rotattion for "this", reflect to component
	worldTransform.SetRot(q);
	_InnerReflectRot();

	// We have parent, update relative transform
	if (parent)
		relTransform.SetRot(worldTransform.GetRot() * mm::inverse(parent->worldTransform.GetRot()));
		
	// Rotate childs also, "this" rotation causes childs to move in world space, not just rot
	for (auto& c : childs)
		_InnerRotChildRecursively(c, dRot, worldTransform.GetPos());
}

void WorldComponent::SetScaleLocal(const mm::vec3& v)
{
	mm::vec3 dScale = v / worldTransform.GetScaleLocal();

	worldTransform.SetSkew(mm::create_scale(v));
	_InnerReflectSkew();

	// If we got parent update relative skew...
	if(parent)
		relTransform.SetSkew(worldTransform.GetSkew() * mm::inverse(parent->worldTransform.GetSkew()));

	// dScale - el elõre szorzódik a child world skew....
	//Scale childs
	for (auto& c : childs)
		_InnerScaleChildRecursively(c, worldTransform.GetPos(), worldTransform.GetRot(), worldTransform.GetScaleLocal(), worldTransform.GetSkew(), dScale, mm::inverse(worldTransform.GetRot()));
}

void WorldComponent::SetRelPos(const mm::vec3& v)
{
	mm::vec3 relDMove = v - relTransform.GetPos();

	// Update relative position
	relTransform.SetPos(v);

	// Calculate the worldSpace delta move to move childs easily, their relative transform cant change
	mm::vec3 dMove = mm::rotate_vector(GetRot(), GetSkew() * relDMove);

	// Relative moving also changes world moving, update "this"
	worldTransform.Move(dMove);
	_InnerReflectPos();

	// dMove is in world Space now, move childs
	for(auto& c : childs)
		_InnerMoveChildRecursively(c, dMove);
}

void WorldComponent::SetRelRot(const mm::quat& q)
{
	mm::quat relRot = q * mm::inverse(relTransform.GetRot());

	// Update relative rotation
	relTransform.SetRot(q);

	// Calculate the worldSpace delta rot to rot childs easily, their relative transform cant change
	mm::quat dRot = relRot * GetRot();

	// Relative rotating also changes world rotation..
	worldTransform.Rot(dRot);
	_InnerReflectRot();

	// dRot is in world Space now, rotate childs
	for (auto& c : childs)
		_InnerRotChildRecursively(c, dRot, worldTransform.GetPos());
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
	SetRot(GetRot() * q);
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
	SetRelRot(GetRelRot() * q);
}

void WorldComponent::ScaleRel(const mm::vec3& v)
{
	assert(0);
}

const Transform3D WorldComponent::GetTransform() const
{
	Transform3D worldTransform;
		worldTransform.SetPos(GetPos());
		worldTransform.SetRot(GetRot());
		worldTransform.SetScaleLocal(GetScaleLocal());
		worldTransform.SetSkew(GetSkew());
	return worldTransform;
}

void WorldComponent::_InnerMoveChildRecursively(WorldComponent* child, const mm::vec3& dMove)
{
	child->worldTransform.Move(dMove);
	child->_InnerReflectPos();

	for(auto& c : child->childs)
		_InnerMoveChildRecursively(c, dMove);
}

void WorldComponent::_InnerRotChildRecursively(WorldComponent* child, const mm::quat& dRot, const mm::vec3& transformRootPos)
{
	// Rotate child, reflect it to component
	child->worldTransform.Rot(dRot);
	child->_InnerReflectRot();

	// Child rotations causes position change...
	child->worldTransform.SetPos(transformRootPos + mm::rotate_vector(dRot, child->worldTransform.GetPos() - transformRootPos));
	child->_InnerReflectPos();

	// Rot childs
	for (auto& c : child->childs)
		_InnerRotChildRecursively(c, dRot, transformRootPos);
}

void WorldComponent::_InnerScaleChildRecursively(WorldComponent* child, const mm::vec3& parentPos, const mm::quat& parentRot, const mm::vec3& parentLocalScale,  const mm::mat3& parentSkew, const mm::vec3& dScale, const mm::quat& rootRotInverse)
{
	// Set child new skew
	mm::mat4 rotRelToRoot = child->worldTransform.GetRot() * rootRotInverse;
	mm::mat4 invRotRelToRoot = mm::inverse(child->worldTransform.GetRot() * rootRotInverse);

	child->worldTransform.SetSkew(mm::mat4(child->worldTransform.GetSkew()) * invRotRelToRoot * mm::create_scale(dScale) * rotRelToRoot);
	child->_InnerReflectSkew();

	// Set child new position
	child->worldTransform.SetPos(parentPos + mm::rotate_vector(parentRot, child->relTransform.GetPos() * parentLocalScale));
	child->_InnerReflectPos();

	// Skew childs
	for (auto& c : child->childs)
		_InnerScaleChildRecursively(c, child->worldTransform.GetPos(), child->worldTransform.GetRot(), child->worldTransform.GetScaleLocal(), child->worldTransform.GetSkew(), dScale, rootRotInverse);
}