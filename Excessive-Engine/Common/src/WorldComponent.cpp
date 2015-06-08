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

void WorldComponent::updateAfterPhysicsSimulate()
{

}

WorldComponent* WorldComponent::attach(WorldComponent* c)
{
	if (c == this)
		return nullptr;

	c->setParent(this);

	return c;
}

WorldComponent* WorldComponent::attachTo(WorldComponent* c)
{
	setParent(c);
	return c;
}

WorldComponent* WorldComponent::detach()
{
	WorldComponent* oldParent = parent;
	
	setParent(nullptr);

	return oldParent;
}

WorldComponent* WorldComponent::setParent(WorldComponent* c)
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
		relativeTransform.setRot(mm::inverse(parent->worldTransform.getRot()) * worldTransform.getRot());
		relativeTransform.setScale(worldTransform.getScale() / parent->worldTransform.getScale());
		relativeTransform.setPos(mm::rotate_vector(mm::inverse(parent->worldTransform.getRot()), (worldTransform.getPos() - parent->worldTransform.getPos()) / parent->worldTransform.getScale()));

		parent->childs.push_back(this);
	}
	else // become root component
	{
		relativeTransform = worldTransform;
	}
	
	return c;
}

void WorldComponent::setTransform(const Transform3D& t)
{
	mm::quat q = t.getRot();

	if (getParent())
		_innerRefreshTransform(q * mm::inverse(getParent()->getRot()), q * mm::inverse(worldTransform.getRot()), t.getScale() / worldTransform.getScale());
	else
		_innerRefreshTransform(q, mm::inverse(worldTransform.getRot()) * q, t.getScale() / worldTransform.getScale());

	worldTransform = t;
}

void WorldComponent::setPos(const mm::vec3& v)
{
	Transform3D tmp = worldTransform;
		tmp.setPos(v);
	setTransform(tmp);
}

void WorldComponent::setRot(const mm::quat& q)
{
	Transform3D tmp = worldTransform;
		tmp.setRot(q);
	setTransform(tmp);
}

void WorldComponent::setScale(const mm::vec3& v)
{
	Transform3D tmp = worldTransform;
		tmp.setScale(v);
	setTransform(tmp);
}

void WorldComponent::move(const mm::vec3& v)
{
	setPos(getPos() + v);
}

void WorldComponent::rot(const mm::quat& q)
{
	// TODO
	setRot(getRot() * q);
}

void WorldComponent::scale(const mm::vec3& v)
{
	setScale(getScale() * v);
}

void WorldComponent::moveRel(const mm::vec3& v)
{
	if (parent)
	{
		relativeTransform.move(v);

		Transform3D tmp = worldTransform;
			tmp.setPos(mm::rotate_vector(parent->getRot(), relativeTransform.getPos() / parent->getScale()));
		setTransform(tmp);
	}
	else
	{
		move(v);
	}
}

void WorldComponent::rotRel(const mm::quat& q)
{
	if (parent)
	{
		Transform3D tmp = worldTransform;
		tmp.setRot(relativeTransform.getRot() * q * worldTransform.getRot());
		setTransform(tmp);
	}
	else
	{
		rot(q);
	}
}

void WorldComponent::scaleRel(const mm::vec3& v)
{
	if (parent)
	{
		Transform3D tmp = worldTransform;
		tmp.setScale(relativeTransform.getScale() * v * worldTransform.getScale());
		setTransform(tmp);
	}
	else
	{
		scale(v);
	}
	
}

const Transform3D WorldComponent::getTransform() const
{
	Transform3D worldTransform;
		worldTransform.setPos(getPos());
		worldTransform.setRot(getRot());
		worldTransform.setScale(getScale());
	return worldTransform;
}

void WorldComponent::_innerRefreshTransform(const mm::quat& relRot, const mm::quat& worldDeltaRot, const mm::vec3& worldDeltaScale)
{
	mm::quat inverseRot;
	if (childs.size() != 0)
		inverseRot = mm::inverse(relativeTransform.getRot());

	relativeTransform.setRot(relRot);

	worldTransform.rot(worldDeltaRot);
	_innerUpdateRot();

	for (auto& comp : childs)
	{
		comp->_innerRefreshTransform(inverseRot * relRot, worldDeltaRot, worldDeltaScale); // TODO bad quat multip order
		
		comp->worldTransform.setScale(comp->relativeTransform.getScale() * worldTransform.getScale() * worldDeltaScale);
		comp->_innerUpdateScale();

		comp->worldTransform.setPos(worldTransform.getPos() + mm::rotate_vector(worldTransform.getRot(), comp->relativeTransform.getPos() *  worldTransform.getScale() * worldDeltaScale));
		comp->_innerUpdatePos();
	}
}