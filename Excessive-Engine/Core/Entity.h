#pragma once
#include "WorldComponent.h"
#include <functional>
#include "SupportLibrary\BasicTypes.h"
#include "RigidBodyComponent.h"
#include <algorithm>

void CollectComponentsRecursively(WorldComponent* c, std::vector<WorldComponent*>& comps);

class Entity
{
public:
	Entity();

	__inline WorldComponent* AttachTo(WorldComponent* c) { return rootComp->AttachTo(c); }
	__inline WorldComponent* Attach(WorldComponent* c) { return rootComp->Attach(c); }
	__inline WorldComponent* Detach() { return rootComp->Detach(); }

	__inline bool AddForce(const mm::vec3& force, const mm::vec3& relPos = { 0, 0, 0 })
	{
		return RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->AddForce(force, relPos);
		});
	}

	template<class T>
	__inline bool RunLambdaOnComponents(const std::function<void(T*)>& lambda) const
	{
		auto& specTypeComps = GetComponents<T>();

		for (auto& comp : specTypeComps)
			lambda(comp);

		return specTypeComps.size() != 0;
	}

	__inline bool SetTrigger(bool bTrigger)
	{
		return RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->SetTrigger(bTrigger);
		});
	}

	__inline bool SetGravityScale(float s)
	{
		return RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->SetGravityScale(s);
		});
	}

	__inline bool SetCollisionGroup(i64 ID)
	{
		return RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->SetCollisionGroup(ID);
		});
	}

	__inline bool SetAngularFactor(float f)
	{
		return RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->SetAngularFactor(f);
		});
	}

	__inline bool SetKinematic(bool b)
	{
		return RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->SetKinematic(b);
		});
	}

	__inline bool SetVelocity(const mm::vec3& v)
	{
		return RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->SetVelocity(v);
		});
	}

	__inline WorldComponent* SetParent(WorldComponent* c) { return rootComp->SetParent(c); }

	__inline void SetRootComp(WorldComponent* c) { rootComp = c; }

	__inline void SetPos(const mm::vec3& v) { rootComp->SetPos(v); }
	__inline void SetRot(const mm::quat& q) { rootComp->SetRot(q); }
	__inline void SetScaleLocal(const mm::vec3& v) { rootComp->SetScaleLocal(v); }

	__inline void Move(const mm::vec3& v) { rootComp->Move(v); }
	__inline void Rot(const mm::quat& q) { rootComp->Rot(q); }
	__inline void Scale(const mm::vec3& v) { rootComp->Scale(v); }
	__inline void ScaleLocal(const mm::vec3& v) { rootComp->ScaleLocal(v); }

	__inline void SetRelPos(const mm::vec3& v) { rootComp->SetRelPos(v); }
	__inline void SetRelRot(const mm::quat& q) { rootComp->SetRelRot(q); }
	__inline void SetRelScale(const mm::vec3& v) { rootComp->SetRelScale(v); }
	__inline void MoveRel(const mm::vec3& v) { rootComp->MoveRel(v); }
	__inline void RotRel(const mm::quat& q) { rootComp->RotRel(q); }
	__inline void ScaleRel(const mm::vec3& v) { rootComp->ScaleRel(v); }

	__inline WorldComponent* GetParent() const { return rootComp->GetParent(); }

	__inline const mm::vec3  GetScaleLocal() const		{ return rootComp->GetScaleLocal(); }
	__inline const mm::mat3& GetSkew()		 const		{ return rootComp->GetSkew(); }
	__inline const mm::vec3& GetPos()		 const		{ return rootComp->GetPos(); }
	__inline const mm::quat& GetRot()		 const		{ return rootComp->GetRot(); }

	__inline const mm::vec3  GetRelScaleLocal() const	{ return rootComp->GetScaleLocal(); }
	__inline const mm::vec3& GetRelPos()		const	{ return rootComp->GetPos(); }
	__inline const mm::quat& GetRelRot()		const	{ return rootComp->GetRot(); }


	__inline const Transform3D& GetRelTransform()	const { return rootComp->GetRelTransform(); }
	__inline const Transform3D& GetTransform()		const { return rootComp->GetTransform(); }

	__inline mm::vec3 GetDirFrontNormed()	const { return rootComp->GetDirFrontNormed(); }
	__inline mm::vec3 GetDirBackNormed()	const { return rootComp->GetDirBackNormed(); }
	__inline mm::vec3 GetDirUpNormed()		const { return rootComp->GetDirUpNormed(); }
	__inline mm::vec3 GetDirDownNormed()	const { return rootComp->GetDirDownNormed(); }
	__inline mm::vec3 GetDirRightNormed()	const { return rootComp->GetDirRightNormed(); }
	__inline mm::vec3 GetDirLeftNormed()	const { return rootComp->GetDirLeftNormed(); }


	__inline mm::vec3 GetVelocity() const 
	{ 
		mm::vec3 vel(0,0,0);
		size_t nRigids = 0;
		RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			nRigids++;
			vel += c->GetVelocity();
		});

		if (nRigids == 0)
			return vel;

		vel /= nRigids;
		return vel;
	}

	__inline std::vector<WorldComponent*> GetComponents() const
	{ 
		std::vector<WorldComponent*> result;

		if (rootComp)
			CollectComponentsRecursively(rootComp, result);

		return result;
	}

	template<class T>
	__inline std::vector<T*> GetComponents() const
	{ 
		auto& allComps = GetComponents();

		std::vector<T*> compsWithMatchedType;

		for (auto& c : allComps)
			if (dynamic_cast<T*>(c))
				compsWithMatchedType.push_back((T*)c);

		return compsWithMatchedType;
	}

protected:
	WorldComponent* rootComp;
};