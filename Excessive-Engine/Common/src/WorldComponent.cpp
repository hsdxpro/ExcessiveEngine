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
		relativeTransform = _InnerCalcRelTransform();

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
	mm::quat rot = t.GetRot();

	mm::vec3 dScale = t.GetScale() / worldTransform.GetScale();

	if (GetParent())
	{
		_InnerRefreshTransform(mm::inverse(rot), rot * mm::inverse(GetParent()->GetRot()), rot * mm::inverse(worldTransform.GetRot()), dScale, mm::mat4(worldTransform.GetRot()) * mm::create_scale(dScale), dScale);
	}
	else
	{
		_InnerRefreshTransform(mm::inverse(rot), rot, mm::inverse(worldTransform.GetRot()) * rot, dScale, mm::mat4(worldTransform.GetRot()) * mm::create_scale(dScale), dScale);
	}

	worldTransform = t;
}

void WorldComponent::SetPos(const mm::vec3& v)
{
	Transform3D tmp = worldTransform;
		tmp.SetPos(v);
	SetTransform(tmp);

	relativeTransform = _InnerCalcRelTransform();
}

void WorldComponent::SetRot(const mm::quat& q)
{
	Transform3D tmp = worldTransform;
		tmp.SetRot(q);
	SetTransform(tmp);

	relativeTransform = _InnerCalcRelTransform();
}

void WorldComponent::SetScale(const mm::vec3& v)
{
	if (GetParent())
	{
		//worldTransform.SetSkew(mm::mat3(mm::inverse(GetRelRot())) * GetParent()->GetSkew() * mm::mat3(mm::inverse(GetRelRot())));
		worldTransform.SetSkew(GetParent()->GetSkew() * GetRelSkew() * mm::mat3(mm::create_scale(v / worldTransform.GetScale())));
		_InnerUpdateSkew();
	}
	else
	{
		worldTransform.SetSkew(worldTransform.GetSkew() * mm::mat3(mm::create_scale(v / worldTransform.GetScale())));
		_InnerUpdateSkew();
	}

	Transform3D tmp = worldTransform;
		tmp.SetScale(v);
	SetTransform(tmp);

	relativeTransform = _InnerCalcRelTransform();
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

void WorldComponent::_InnerRefreshTransform(const mm::quat& scaledActorRotInverse, const mm::quat& relRot, const mm::quat& worldDeltaRot, const mm::vec3& worldDeltaScale, const mm::mat4& tmp, mm::vec3 cheat)
{
	mm::quat inverseRot;
	if (childs.size() != 0)
		inverseRot = mm::inverse(relativeTransform.GetRot());

	//relativeTransform.SetRot(relRot);

	worldTransform.Rot(worldDeltaRot);
	_InnerUpdateRot();

	for (auto& comp : childs)
	{ 

		// Csodás skewDir world space irányba akarjuk elnyújtani a dolgokat..
		// Lokálisan ez hogy érint minket ?:O
		//mm::vec3 localSkewDir = mm::rotate_vector(mm::inverse(comp->worldTransform.GetRot()), skewDir);
		
		// Ez a localSkewDir szegényes mert tényleg csak lokálisan fog nyújtani, nem world space - ban...
		// Elkéne érnünk hogy az objektumunkat elõszõr beforgassuk "skewDir bázisra", hogy lokálisan nyújtást érjünk el

		mm::quat deltaRot = comp->GetRot() * scaledActorRotInverse; // comp relative rot to scaledActor
		comp->worldTransform.SetSkew(comp->worldTransform.GetSkew() * mm::mat3(mm::inverse(deltaRot)) * mm::mat3(mm::create_scale(worldDeltaScale)) * mm::mat3(deltaRot));

		// deltaRot = scaledActorRot * inverse(compRot);
		// compSkew = deltaRot * mm::create_scale(worldDeltaScale) * deltaRot;

		//comp->worldTransform.SetSkew(comp->worldTransform.GetSkew() * mm::mat3(mm::create_scale(localSkewDir)));
		// Tehát tudjuk hogy a local scale mi lesz...
		//comp->worldTransform.SetSkew(comp->worldTransform.GetSkew() * mm::mat3(mm::inverse(comp->GetRelRot())) * mm::mat3(mm::create_scale(localSkewDir)) * mm::mat3(comp->GetRelRot()));
		//comp->worldTransform.SetSkew(comp->worldTransform.GetSkew() * mm::mat3(mm::inverse(comp->GetRelRot())) * mm::mat3(mm::create_scale(skewDir)) * mm::mat3(comp->GetRelRot()));

		//comp->worldTransform.SetSkew(GetSkew() * comp->GetRelSkew());
		comp->_InnerUpdateSkew();

		mm::vec3 deltaCheat = { 1, 1, 1 };

		
		comp->worldTransform.SetPos(worldTransform.GetPos() + mm::rotate_vector(worldTransform.GetRot(), (comp->relativeTransform.GetPos()) * GetSkew()));
		comp->_InnerUpdatePos();


		// Elõ kell álítani egy forgást meg scale - t
		comp->_InnerRefreshTransform(scaledActorRotInverse, inverseRot * relRot, worldDeltaRot, worldDeltaScale, tmp, deltaCheat); // TODO bad quat multip order
	}
}

Transform3D&& WorldComponent::_InnerCalcRelTransform()
{
	Transform3D result;
	if (parent)
	{
		// Recalc relative transform...
		result.SetRot(mm::inverse(parent->worldTransform.GetRot()) * worldTransform.GetRot());
		result.SetPos(mm::inverse(parent->GetSkew()) * mm::rotate_vector(mm::inverse(parent->worldTransform.GetRot()), GetPos() - parent->GetPos()));
		result.SetSkew(mm::inverse(parent->GetSkew()) * GetSkew());
	}
	else
	{
		result = worldTransform;
	}
		

	return std::move(result);
}
