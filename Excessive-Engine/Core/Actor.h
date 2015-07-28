#pragma once
#include "Behavior.h"
#include <functional>
#include "PhysicsEngine\PhysicsCommon.h"

class Actor;

// TODO, create something for that struct like CoreCommon.h
struct rCollision
{
	rCollision(): self(0), other(0), selfBody(0), otherBody(0){}

	Actor*  self;
	Actor*  other;

	physics::IRigidBodyEntity* selfBody;
	physics::IRigidBodyEntity* otherBody;

	std::vector<rContactPoint> contacts;
};

class Actor
{
public:
	Actor();

public:
	__inline void AddBehavior(Behavior* b)	{behaviors.push_back(b);}

	__inline WorldComponent* AttachTo(WorldComponent* c) { return entity->AttachTo(c); }
	__inline WorldComponent* Attach(WorldComponent* c) { return entity->Attach(c); }
	__inline WorldComponent* Detach() { return entity->Detach(); }

	template<class T>
	__inline bool RunLambdaOnComponents(const std::function<void(T*)>& lambda) 
	{
		if (entity)
			return entity->RunLambdaOnComponents<T>(lambda);

		return false;
	}

	__inline void SetPendingKill(bool bKill)
	{
		bPendingKill = bKill;
	}

	__inline void SetEntity(Entity* e) 
	{
		entity = e; 
		
		for (auto& b : behaviors)
			b->SetEntity(e); 
	}

	__inline bool SetTrigger(bool bTrigger)
	{ 
		if (entity)
			entity->SetTrigger(true);

		return false;
	}

	__inline bool SetGravityScale(float s)
	{
		if (entity)
			return entity->SetGravityScale(s);

		return false;
	}

	__inline bool SetCollisionGroup(i64 ID)
	{
		if (entity)
			return entity->SetCollisionGroup(ID);

		return false;
	}

	__inline void SetOnUpdate(const std::function<void(float deltaSeconds)>& callb) { onUpdate = callb; }

	__inline void SetOnCollision(const std::function<void(const rCollision& info)>& callb) { onCollision = callb; }
	__inline void SetOnCollisionEnter(const std::function<void(const rCollision& info)>& callb) { onCollisionEnter = callb;}
	__inline void SetOnCollisionExit(const std::function<void(const rCollision& info)>& callb) { onCollisionExit = callb; }
	
	__inline WorldComponent* SetParent(WorldComponent* c) { return entity->SetParent(c); }

	__inline void SetPos(const mm::vec3& v) { entity->SetPos(v); }
	__inline void SetRot(const mm::quat& q) { entity->SetRot(q); }
	__inline void SetScaleLocal(const mm::vec3& v) { entity->SetScaleLocal(v); }

	__inline void Move(const mm::vec3& v) { entity->Move(v); }
	__inline void Rot(const mm::quat& q) { entity->Rot(q); }
	__inline void Scale(const mm::vec3& v) { entity->Scale(v); }
	__inline void ScaleLocal(const mm::vec3& v) { entity->ScaleLocal(v); }

	__inline void SetRelPos(const mm::vec3& v) { entity->SetRelPos(v); }
	__inline void SetRelRot(const mm::quat& q) { entity->SetRelRot(q); }
	__inline void SetRelScale(const mm::vec3& v) { entity->SetRelScale(v); }
	__inline void MoveRel(const mm::vec3& v) { entity->MoveRel(v); }
	__inline void RotRel(const mm::quat& q) { entity->RotRel(q); }
	__inline void ScaleRel(const mm::vec3& v) { entity->ScaleRel(v); }

	__inline WorldComponent* GetParent() const { return entity->GetParent(); }

	__inline const mm::vec3& GetScaleLocal() const		{ return entity->GetScaleLocal(); }
	__inline const mm::mat3& GetSkew()		 const		{ return entity->GetSkew(); }
	__inline const mm::vec3& GetPos()		 const		{ return entity->GetPos(); }
	__inline const mm::quat& GetRot()		 const		{ return entity->GetRot(); }

	__inline const mm::vec3& GetRelScaleLocal() const	{ return entity->GetScaleLocal(); }
	__inline const mm::vec3& GetRelPos()		const	{ return entity->GetPos(); }
	__inline const mm::quat& GetRelRot()		const	{ return entity->GetRot(); }


	__inline const Transform3D& GetRelTransform()	const { return entity->GetRelTransform(); }
	__inline const Transform3D& GetTransform()		const { return entity->GetTransform(); }

	__inline mm::vec3 GetDirFrontNormed()	const { return entity->GetDirFrontNormed(); }
	__inline mm::vec3 GetDirBackNormed()	const { return entity->GetDirBackNormed(); }
	__inline mm::vec3 GetDirUpNormed()		const { return entity->GetDirUpNormed(); }
	__inline mm::vec3 GetDirDownNormed()	const { return entity->GetDirDownNormed(); }
	__inline mm::vec3 GetDirRightNormed()	const { return entity->GetDirRightNormed(); }
	__inline mm::vec3 GetDirLeftNormed()	const { return entity->GetDirLeftNormed(); }

	__inline std::vector<WorldComponent*> GetComponents() 
	{
		if(entity)
			return entity->GetComponents();
		
		return std::vector<WorldComponent*>();
	}

	template<class T>
	__inline std::vector<T*> GetComponents()
	{ 
		if (entity)
			return entity->GetComponents<T>();

		return std::vector<T*>();
	}

	__inline std::vector<Behavior*>& GetBehaviors() {return behaviors;}
	__inline Entity* GetEntity() const {return entity;}

	__inline const std::function<void(float deltaSeconds)>& GetOnUpdate() { return onUpdate; }
	__inline const std::function<void(const rCollision& col)>& GetOnCollision() { return onCollision; }
	__inline const std::function<void(const rCollision& col)>& GetOnCollisionEnter() { return onCollisionEnter; }
	__inline const std::function<void(const rCollision& col)>& GetOnCollisionExit() { return onCollisionExit; }

	__inline bool IsPendingKill() { return bPendingKill; }
protected:
	Entity* entity;

	std::vector<Behavior*> behaviors;

	std::function<void(float deltaSeconds)> onUpdate;
	std::function<void(const rCollision& col)> onCollision;
	std::function<void(const rCollision& col)> onCollisionEnter;
	std::function<void(const rCollision& col)> onCollisionExit;

	// Lifecycle
	bool bPendingKill;
};