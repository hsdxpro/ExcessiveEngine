#pragma once
#include "Behavior.h"
#include <functional>
#include "CoreCommon.h"

class Actor
{
public:
	Actor();

public:
	__inline void AddBehavior(Behavior* b)	{ behaviors.push_back(b); }

	__inline bool AddForce(const mm::vec3& force, const mm::vec3& relPos = { 0, 0, 0 }) { return worldEntity->AddForce(force, relPos); }

	__inline WorldComponent* AttachTo(WorldComponent* c) { return worldEntity->AttachTo(c); }
	__inline WorldComponent* Attach(WorldComponent* c)	 { return worldEntity->Attach(c); }
	__inline WorldComponent* Detach() { return worldEntity->Detach(); }

	template<class T>
	__inline bool RunLambdaOnComponents(const std::function<void(T*)>& lambda) { return worldEntity->RunLambdaOnComponents<T>(lambda); }

	__inline void SetPendingKill(bool bKill) { bPendingKill = bKill; }

	__inline void SetEntity(Entity* e)
	{
		worldEntity = e;

		for (auto& b : behaviors)
			b->SetEntity(e);
	}

	__inline bool SetTrigger(bool bTrigger) { return worldEntity->SetTrigger(true); }

	__inline bool SetGravityScale(float s) { return worldEntity->SetGravityScale(s); }

	__inline bool SetCollisionGroup(i64 ID) { return worldEntity->SetCollisionGroup(ID); }

	__inline bool SetAngularFactor(float f) { return worldEntity->SetAngularFactor(f); }

	__inline bool SetKinematic(bool b) {return worldEntity->SetKinematic(b); }

	__inline bool SetVelocity(const mm::vec3& v) { return worldEntity->SetVelocity(v); }

	__inline void SetOnUpdate(const std::function<void(float deltaSeconds)>& callb) { onUpdate = callb; }

	__inline void SetOnCollision(const std::function<void(const rCollision& info)>& callb)		{ onCollision = callb; }
	__inline void SetOnCollisionEnter(const std::function<void(const rCollision& info)>& callb) { onCollisionEnter = callb;}
	__inline void SetOnCollisionExit(const std::function<void(const rCollision& info)>& callb)	{ onCollisionExit = callb; }
	
	__inline WorldComponent* SetParent(WorldComponent* c) { return worldEntity->SetParent(c); }

	__inline void SetPos(const mm::vec3& v)			{ worldEntity->SetPos(v); }
	__inline void SetRot(const mm::quat& q)			{ worldEntity->SetRot(q); }
	__inline void SetScaleLocal(const mm::vec3& v)	{ worldEntity->SetScaleLocal(v); }

	__inline void Move(const mm::vec3& v)		{ worldEntity->Move(v); }
	__inline void Rot(const mm::quat& q)		{ worldEntity->Rot(q); }
	__inline void Scale(const mm::vec3& v)		{ worldEntity->Scale(v); }
	__inline void ScaleLocal(const mm::vec3& v)	{ worldEntity->ScaleLocal(v); }

	__inline void SetRelPos(const mm::vec3& v)	 { worldEntity->SetRelPos(v); }
	__inline void SetRelRot(const mm::quat& q)	 { worldEntity->SetRelRot(q); }
	__inline void SetRelScale(const mm::vec3& v) { worldEntity->SetRelScale(v); }
	__inline void MoveRel(const mm::vec3& v)	 { worldEntity->MoveRel(v); }
	__inline void RotRel(const mm::quat& q)		 { worldEntity->RotRel(q); }
	__inline void ScaleRel(const mm::vec3& v)	 { worldEntity->ScaleRel(v); }

	__inline WorldComponent* GetParent() const { return worldEntity->GetParent(); }

	__inline const mm::vec3  GetScaleLocal() const		{ return worldEntity->GetScaleLocal(); }
	__inline const mm::mat3& GetSkew()		 const		{ return worldEntity->GetSkew(); }
	__inline const mm::vec3& GetPos()		 const		{ return worldEntity->GetPos(); }
	__inline const mm::quat& GetRot()		 const		{ return worldEntity->GetRot(); }

	__inline const mm::vec3  GetRelScaleLocal() const	{ return worldEntity->GetScaleLocal(); }
	__inline const mm::vec3& GetRelPos()		const	{ return worldEntity->GetPos(); }
	__inline const mm::quat& GetRelRot()		const	{ return worldEntity->GetRot(); }


	__inline const Transform3D& GetRelTransform()	const { return worldEntity->GetRelTransform(); }
	__inline const Transform3D& GetTransform()		const { return worldEntity->GetTransform(); }

	__inline mm::vec3 GetDirFrontNormed()	const { return worldEntity->GetDirFrontNormed(); }
	__inline mm::vec3 GetDirBackNormed()	const { return worldEntity->GetDirBackNormed(); }
	__inline mm::vec3 GetDirUpNormed()		const { return worldEntity->GetDirUpNormed(); }
	__inline mm::vec3 GetDirDownNormed()	const { return worldEntity->GetDirDownNormed(); }
	__inline mm::vec3 GetDirRightNormed()	const { return worldEntity->GetDirRightNormed(); }
	__inline mm::vec3 GetDirLeftNormed()	const { return worldEntity->GetDirLeftNormed(); }

	__inline mm::vec3 GetVelocity() const { return worldEntity->GetVelocity(); }

	__inline const std::vector<WorldComponent*> GetComponents() const { return worldEntity->GetComponents(); }
	__inline void GetComponents(std::vector<WorldComponent*>& allComp) const { worldEntity->GetComponents(allComp); }

	template<class T>
	__inline std::vector<T*> GetComponents() const { return worldEntity->GetComponents<T>(); }

	__inline std::vector<Behavior*>& GetBehaviors() {return behaviors;}
	__inline Entity* GetEntity() const { return worldEntity; }

	__inline const std::function<void(float deltaSeconds)>& GetOnUpdate() { return onUpdate; }
	__inline const std::function<void(const rCollision& col)>& GetOnCollision() { return onCollision; }
	__inline const std::function<void(const rCollision& col)>& GetOnCollisionEnter() { return onCollisionEnter; }
	__inline const std::function<void(const rCollision& col)>& GetOnCollisionExit() { return onCollisionExit; }

	__inline bool IsPendingKill() { return bPendingKill; }
protected:
	Entity* worldEntity;

	std::vector<Behavior*> behaviors;

	std::function<void(float deltaSeconds)> onUpdate;
	std::function<void(const rCollision& col)> onCollision;
	std::function<void(const rCollision& col)> onCollisionEnter;
	std::function<void(const rCollision& col)> onCollisionExit;

	// Lifecycle
	bool bPendingKill;
};