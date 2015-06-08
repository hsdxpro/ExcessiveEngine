#pragma once
// Represent those components which can be placed in a 3D world (pos, rot, scale), attachable to Actor, and to itself
#include <vector>
#include "BasicTypes.h"
#include "mymath\mymath.h"
#include "Component.h"
#include "Transform3D.h"

class WorldComponent : public Component
{
public:
	WorldComponent();
	~WorldComponent();

public:
	virtual void updateAfterPhysicsSimulate();

	WorldComponent* attachTo(WorldComponent* c);
	WorldComponent* attach(WorldComponent* c);
	WorldComponent* detach();

	WorldComponent* setParent(WorldComponent* c);

	void setTransform(const Transform3D& t);

	virtual void setPos(const mm::vec3& v);
	virtual void setRot(const mm::quat& q);
	virtual void setScale(const mm::vec3& v);

	void move(const mm::vec3& v);
	void rot(const mm::quat& q);
	void scale(const mm::vec3& v);

	void moveRel(const mm::vec3& v);
	void rotRel(const mm::quat& q);
	void scaleRel(const mm::vec3& v);

	__inline WorldComponent* getParent() const { return parent; }

	__inline virtual const mm::vec3 getPos() const { return worldTransform.getPos(); }
	__inline virtual const mm::quat getRot() const { return worldTransform.getRot(); }
	__inline virtual const mm::vec3 getScale() const { return worldTransform.getScale(); }

	__inline const mm::vec3& getRelPos() const { return relativeTransform.getPos(); }
	__inline const mm::quat& getRelRot() const { return relativeTransform.getRot(); }
	__inline const mm::vec3& getRelScale() const { return relativeTransform.getScale(); }

	__inline const Transform3D& getRelativeTransform() const { return relativeTransform; }
	const Transform3D getTransform() const;

protected:
	virtual void _innerUpdatePos() = 0;
	virtual void _innerUpdateRot() = 0;
	virtual void _innerUpdateScale() = 0;

	//void _innerSetRelPos(const mm::vec3& relPos, const mm::vec3& worldDeltaMove);
	void _innerRefreshTransform(const mm::quat& relRot, const mm::quat& worldDeltaRot, const mm::vec3& worldDeltaScale);
	//void _innerSetRelScale(const mm::vec3& relScale, const mm::vec3& worldDeltaScale);

protected:
	std::vector<WorldComponent*> childs;
	WorldComponent* parent;

	Transform3D worldTransform;
	Transform3D relativeTransform;
};