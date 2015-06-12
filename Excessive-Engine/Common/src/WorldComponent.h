#pragma once
// Represent those components which can be placed in a 3D world (pos, rot, scale), attachable to Actor, and to itself
#include <vector>
#include "BasicTypes.h"
#include "mymath\mymath.h"
#include "Transform3D.h"

class WorldComponent
{
public:
	WorldComponent();
	~WorldComponent();

public:
	virtual void UpdateAfterPhysicsSimulate();

	WorldComponent* AttachTo(WorldComponent* c);
	WorldComponent* Attach(WorldComponent* c);
	WorldComponent* Detach();

	WorldComponent* SetParent(WorldComponent* c);

	void SetTransform(const Transform3D& t);

	virtual void SetPos(const mm::vec3& v);
	virtual void SetRot(const mm::quat& q);
	virtual void SetScale(const mm::vec3& v);

	void Move(const mm::vec3& v);
	void Rot(const mm::quat& q);
	void Scale(const mm::vec3& v);

	void MoveRel(const mm::vec3& v);
	void RotRel(const mm::quat& q);
	void ScaleRel(const mm::vec3& v);

	__inline WorldComponent* GetParent() const { return parent; }

	__inline virtual const mm::vec3 GetPos() const { return worldTransform.GetPos(); }
	__inline virtual const mm::quat GetRot() const { return worldTransform.GetRot(); }
	__inline virtual const mm::vec3 GetScale() const { return worldTransform.GetScale(); }
	__inline virtual const mm::mat3 GetSkew() const { return worldTransform.GetSkew(); }

	__inline const mm::vec3& GetRelPos() const { return relativeTransform.GetPos(); }
	__inline const mm::quat& GetRelRot() const { return relativeTransform.GetRot(); }
	__inline const mm::vec3& GetRelScale() const { return relativeTransform.GetScale(); }

	__inline const Transform3D& GetRelativeTransform() const { return relativeTransform; }
	const Transform3D GetTransform() const;

protected:
	virtual void _InnerUpdatePos() = 0;
	virtual void _InnerUpdateRot() = 0;
	virtual void _InnerUpdateScale() = 0;
	virtual void _InnerUpdateSkew() = 0;

	//void _innerSetRelPos(const mm::vec3& relPos, const mm::vec3& worldDeltaMove);
	void _InnerRefreshTransform(const mm::quat& relRot, const mm::quat& worldDeltaRot, const mm::vec3& worldDeltaScale, const mm::mat4& tmp, mm::vec3 cheat = {1,1,1});
	//void _innerSetRelScale(const mm::vec3& relScale, const mm::vec3& worldDeltaScale);

protected:
	std::vector<WorldComponent*> childs;
	WorldComponent* parent;

	Transform3D worldTransform;
	Transform3D relativeTransform;
};