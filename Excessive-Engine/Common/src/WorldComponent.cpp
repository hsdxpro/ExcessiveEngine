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
		relativeTransform.SetRot(mm::inverse(parent->worldTransform.GetRot()) * worldTransform.GetRot());
		relativeTransform.SetScale(worldTransform.GetScale() / parent->worldTransform.GetScale());
		relativeTransform.SetPos(mm::rotate_vector(mm::inverse(parent->worldTransform.GetRot()), (worldTransform.GetPos() - parent->worldTransform.GetPos()) / parent->worldTransform.GetScale()));

		// TODO...
		mm::mat3 parentRotWithScale = mm::mat4(parent->worldTransform.GetRot()) * mm::create_scale(parent->worldTransform.GetScale()) * mm::mat4(parent->worldTransform.GetSkew());
		mm::mat3 childRotWithScale = mm::mat4(worldTransform.GetRot()) * mm::create_scale(worldTransform.GetScale())* mm::mat4(worldTransform.GetSkew());;
		auto rel = mm::inverse(parentRotWithScale) * childRotWithScale;
		relativeTransform.SetSkew(rel);

		parent->childs.push_back(this);
	}
	else // become root component
	{
		relativeTransform = worldTransform;
	}
	
	return c;
}

void WorldComponent::SetTransform(const Transform3D& t)
{
	mm::quat q = t.GetRot();

	if (GetParent())
	{
		mm::vec3 dScale = t.GetScale() / worldTransform.GetScale();
		_InnerRefreshTransform(q * mm::inverse(GetParent()->GetRot()), q * mm::inverse(worldTransform.GetRot()), dScale, mm::mat4(worldTransform.GetRot()) * mm::create_scale(dScale), dScale);
	}
	else
	{
		mm::vec3 dScale = t.GetScale() / worldTransform.GetScale();
		_InnerRefreshTransform(q, mm::inverse(worldTransform.GetRot()) * q, dScale, mm::mat4(worldTransform.GetRot()) * mm::create_scale(dScale), dScale);
	}
		

	worldTransform = t;
}

void WorldComponent::SetPos(const mm::vec3& v)
{
	Transform3D tmp = worldTransform;
		tmp.SetPos(v);
	SetTransform(tmp);
}

void WorldComponent::SetRot(const mm::quat& q)
{
	Transform3D tmp = worldTransform;
		tmp.SetRot(q);
	SetTransform(tmp);
}

void WorldComponent::SetScale(const mm::vec3& v)
{
	Transform3D tmp = worldTransform;
		tmp.SetScale(v);
	SetTransform(tmp);
}

void WorldComponent::Move(const mm::vec3& v)
{
	SetPos(GetPos() + v);
}

void WorldComponent::Rot(const mm::quat& q)
{
	// TODO
	SetRot(GetRot() * q);
}

void WorldComponent::Scale(const mm::vec3& v)
{
	SetScale(GetScale() * v);
}

void WorldComponent::MoveRel(const mm::vec3& v)
{
	if (parent)
	{
		relativeTransform.Move(v);

		Transform3D tmp = worldTransform;
			tmp.SetPos(mm::rotate_vector(parent->GetRot(), relativeTransform.GetPos() / parent->GetScale()));
		SetTransform(tmp);
	}
	else
	{
		Move(v);
	}
}

void WorldComponent::RotRel(const mm::quat& q)
{
	if (parent)
	{
		Transform3D tmp = worldTransform;
		tmp.SetRot(relativeTransform.GetRot() * q * worldTransform.GetRot());
		SetTransform(tmp);
	}
	else
	{
		Rot(q);
	}
}

void WorldComponent::ScaleRel(const mm::vec3& v)
{
	if (parent)
	{
		Transform3D tmp = worldTransform;
		tmp.SetScale(relativeTransform.GetScale() * v * worldTransform.GetScale());
		SetTransform(tmp);
	}
	else
	{
		Scale(v);
	}
	
}

const Transform3D WorldComponent::GetTransform() const
{
	Transform3D worldTransform;
		worldTransform.SetPos(GetPos());
		worldTransform.SetRot(GetRot());
		worldTransform.SetScale(GetScale());
		worldTransform.SetSkew(GetSkew());
	return worldTransform;
}

void WorldComponent::_InnerRefreshTransform(const mm::quat& relRot, const mm::quat& worldDeltaRot, const mm::vec3& worldDeltaScale, const mm::mat4& tmp, mm::vec3 cheat)
{
	mm::quat inverseRot;
	if (childs.size() != 0)
		inverseRot = mm::inverse(relativeTransform.GetRot());

	relativeTransform.SetRot(relRot);

	worldTransform.Rot(worldDeltaRot);
	_InnerUpdateRot();

	for (auto& comp : childs)
	{ 
		mm::mat4 par = mm::mat4(worldTransform.GetRot()) * mm::mat4(worldTransform.GetSkew()) * mm::create_scale(cheat);
		//mm::mat4 par = mm::mat4(worldTransform.GetRot()) * mm::create_scale(relativeTransform.GetScale());
		mm::mat4 skew = par * mm::mat4(comp->relativeTransform.GetSkew());
		//comp->worldTransform.SetSkew(mm::mat4(worldTransform.GetSkew()) * mm::mat4(comp->relativeTransform.GetSkew()));
		comp->worldTransform.SetSkew(skew);
		comp->_InnerUpdateSkew();

		// Gyerek local scale - ja mikor változik??
		//comp->worldTransform.SetScale(comp->relativeTransform.GetScale() * worldTransform.GetScale() * worldDeltaScale);
		//comp->_InnerUpdateScale();

		/// PLS
		// TODO...
		mm::mat3 parentRotWithScale = mm::mat4(worldTransform.GetRot()) * mm::create_scale(worldTransform.GetScale()) * mm::mat4(worldTransform.GetSkew());
		mm::mat3 childRotWithScale = mm::mat4(comp->worldTransform.GetRot()) * mm::create_scale(comp->worldTransform.GetScale())* mm::mat4(comp->worldTransform.GetSkew());;
		auto rel = mm::inverse(parentRotWithScale) * childRotWithScale;
		comp->relativeTransform.SetSkew(rel);


		

		mm::vec3 deltaCheat = { 1, 1, 1 };
		//mm::vec3 tt = tmp;
		//tt /= worldDeltaScale;
		//if (tt.length() < 1)
		//	mm::normalize(tt);

		// Elõ kell álítani egy forgást meg scale - t
		comp->_InnerRefreshTransform(inverseRot * relRot, worldDeltaRot, worldDeltaScale, tmp, deltaCheat); // TODO bad quat multip order

		comp->worldTransform.SetPos(worldTransform.GetPos() + mm::rotate_vector(worldTransform.GetRot(), comp->relativeTransform.GetPos() *  worldTransform.GetScale() * worldDeltaScale));
		comp->_InnerUpdatePos();
	}
}