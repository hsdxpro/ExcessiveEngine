#pragma once
#include "Behavior.h"
#include "CoreCommon.h"
#include "WorldComponent.h"
#include "RigidBodyComponent.h"

#include <functional>

class Actor
{
public:
	Actor(WorldComponent* rootComp);

public:
	__inline void AddBehavior(Behavior* b)	{ behaviors.push_back(b); }

	__inline void AttachTo(WorldComponent* c) { rootComp->AttachTo(c); }
	__inline void AttachTo(Actor* a)
	{ 
		Detach();

		if (parent != a)
		{
			a->childs.push_back(this);
			parent = a;
		}
		// rootComp->AttachTo(a->rootComp);

		// Actor0
		// - comp0
		//	- comp00

		// Actor1
		// - comp1
		//	- comp11

		// Actor0
		// comp0
		//	- comp00
		//	- comp1
		//		- comp11

		// comp1->Detach() // if(dynamic_cast<Actor*>(parent)) ((Actor*)parent)->DetachChild(this);

		//Detach();
		//
		//if (parent != a)
		//{
		//	a->childs.push_back(this);
		//	parent = a;
		//
		//	// Recalc relative transform
		//	relTransform.SetRot(transform.GetRot() * mm::inverse(parent->transform.GetRot()));
		//	relTransform.SetScaleLocal(transform.GetScaleLocal() / parent->transform.GetScaleLocal());
		//	relTransform.SetPos(mm::rotate_vector(mm::inverse(parent->transform.GetRot()), (transform.GetPos() - parent->transform.GetPos()) / parent->transform.GetScaleLocal()));
		//	relTransform.SetSkew(transform.GetSkew() * mm::inverse(parent->transform.GetSkew()));
		//}
	}

	__inline void Attach(WorldComponent* c)	 {  rootComp->Attach(c); }
	__inline void Attach(Actor* a) { a->AttachTo(this); }

	__inline Actor* Detach()
	{
		Actor* savedParent = parent;

		if (parent)
			parent->DetachChild(this);

		return savedParent;
	}

	__inline bool DetachChild(Actor* a)
	{
		rootComp->DetachChild(a->GetRootComp());

		a->parent = nullptr;

		for (size_t i = 0; i < childs.size(); i++)
		{
			if (childs[i] == a)
			{
				// childs[i] not last element, move data front from back
				if (i + 1 < childs.size())
					memmove(&childs[i], &childs[i + 1], childs.size() - 1 - i);

				childs.resize(childs.size() - 1);

				//a->rootComp->SetRelTransform(a->GetTransform());
				// Recalc actor relative transform
				return true;
			}
		}
		return false;
	}

	template<class T>
	__inline void RunLambdaOnComponents(const std::function<void(T*)>& lambda)
	{ 
		std::function<void(WorldComponent* c, const std::function<void(T*)>& lambda)> recursiveFunc;
		recursiveFunc = [&](WorldComponent* c, const std::function<void(T*)>& lambda)
		{
			if (dynamic_cast<T*>(c))
				lambda((T*)c);

			for (auto& child : c->GetChilds())
				recursiveFunc(child, lambda);
		};
		recursiveFunc(rootComp, lambda);
	}

	__inline void AddForce(const mm::vec3& force, const mm::vec3& relPos = { 0, 0, 0 })
	{
		RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->AddForce(force, relPos);
		});
	}

	__inline void SetName(const std::string& s) { name = s; }

	// TODO call Core::Destroy
	__inline void Kill() { bKilled = true; }

	__inline void SetTrigger(bool bTrigger)
	{ 
		RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->SetTrigger(bTrigger);
		});
	}

	__inline void SetGravityScale(float s)
	{
		RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->SetGravityScale(s);
		});
	}

	__inline void SetCollisionGroup(i64 ID)
	{ 
		RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->SetCollisionGroup(ID);
		});
	}

	__inline void SetAngularFactor(float f)
	{ 
		RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->SetAngularFactor(f);
		});
	}

	__inline void SetKinematic(bool b)
	{
		RunLambdaOnComponents<RigidBodyComponent>([&](RigidBodyComponent* c)
		{
			c->SetKinematic(b);
		});
	}

	__inline void SetVelocity(const mm::vec3& v) 
	{ 
		if (rootComp->IsRigidBody())
		{
			rootComp->AsRigidBody()->SetVelocity(v);
		}
	}

	__inline void SetOnUpdate(const std::function<void(float deltaSeconds)>& callb) { onUpdate = callb; }

	__inline void SetOnCollision(const std::function<void(const rCollision& info)>& callb)		{ onCollision = callb; }
	__inline void SetOnCollisionEnter(const std::function<void(const rCollision& info)>& callb) { onCollisionEnter = callb;}
	__inline void SetOnCollisionExit(const std::function<void(const rCollision& info)>& callb)	{ onCollisionExit = callb; }

	__inline void SetPos(const mm::vec3& v)			{ rootComp->SetPos(v); }
	__inline void SetRot(const mm::quat& q)			{ rootComp->SetRot(q); }
	__inline void SetScaleLocal(const mm::vec3& v)	{ rootComp->SetScaleLocal(v); }
	
	__inline void Move(const mm::vec3& v)		{ rootComp->Move(v); }
	__inline void Rot(const mm::quat& q)		{ rootComp->Rot(q); }
	__inline void Scale(const mm::vec3& v)		{ rootComp->Scale(v); }
	__inline void ScaleLocal(const mm::vec3& v)	{ rootComp->ScaleLocal(v); }
	
	__inline void SetRelPos(const mm::vec3& v)	 { rootComp->SetRelPos(v); }
	__inline void SetRelRot(const mm::quat& q)	 { rootComp->SetRelRot(q); }
	__inline void SetRelScale(const mm::vec3& v) { rootComp->SetRelScale(v); }
	__inline void MoveRel(const mm::vec3& v)	 { rootComp->MoveRel(v); }
	__inline void RotRel(const mm::quat& q)		 { rootComp->RotRel(q); }
	__inline void ScaleRel(const mm::vec3& v)	 { rootComp->ScaleRel(v); }
	
	//__inline Entity* GetParent() const { return worldEntity->GetParent(); }
	
	__inline const mm::vec3  GetScaleLocal() const		{ return rootComp->GetScaleLocal(); }
	__inline const mm::mat3& GetSkew()		 const		{ return rootComp->GetSkew(); }
	__inline const mm::vec3& GetPos()		 const		{ return rootComp->GetPos(); }
	__inline const mm::quat& GetRot()		 const		{ return rootComp->GetRot(); }
	
	__inline const mm::vec3  GetRelScaleLocal() const	{ return rootComp->GetScaleLocal(); }
	__inline const mm::vec3& GetRelPos()		const	{ return rootComp->GetPos(); }
	__inline const mm::quat& GetRelRot()		const	{ return rootComp->GetRot(); }
	
	
	__inline const Transform3D& GetRelTransform()	const { return rootComp->GetRelTransform(); }
	__inline const Transform3D& GetTransform()		const { return rootComp->GetTransform(); }
	
	__inline mm::vec3 GetFrontDirNormed()	const { return rootComp->GetFrontDirNormed(); }
	__inline mm::vec3 GetBackDirNormed()	const { return rootComp->GetBackDirNormed(); }
	__inline mm::vec3 GetUpDirNormed()		const { return rootComp->GetUpDirNormed(); }
	__inline mm::vec3 GetDownDirNormed()	const { return rootComp->GetDownDirNormed(); }
	__inline mm::vec3 GetRightDirNormed()	const { return rootComp->GetRightDirNormed(); }
	__inline mm::vec3 GetLeftDirNormed()	const { return rootComp->GetLeftDirNormed(); }

	// TODO
	__inline mm::vec3 GetVelocity() 
	{ 
		if (rootComp->IsRigidBody())
		{
			return rootComp->AsRigidBody()->GetVelocity();
		}
		return mm::vec3(0, 0, 0);
	}

	__inline const std::vector<WorldComponent*> GetComponents() 
	{ 
		std::function<void(WorldComponent*, std::vector<WorldComponent*>& comps_out)> collectCompsRecursively;
		collectCompsRecursively = [&](WorldComponent* c, std::vector<WorldComponent*>& comps_out)
		{
			comps_out.push_back(c);

			for (auto& child : c->GetChilds())
				collectCompsRecursively(child, comps_out);
		};

		std::vector<WorldComponent*> comps;
		collectCompsRecursively(rootComp, comps);

		return comps;
	}

	//__inline void GetComponents(std::vector<WorldComponent*>& allComp) 
	//{ 
	//	static std::function<void(WorldComponent*)> collectCompsRecursively  = [&](WorldComponent* c)
	//	{
	//		allComp.push_back(c);
	//
	//		for (auto& child : c->GetChilds())
	//			collectCompsRecursively(child);
	//	};
	//
	//	collectCompsRecursively(rootComp);
	//}

	template<class T>
	__inline std::vector<T*> GetComponents() const 
	{ 
		std::function<void(WorldComponent* c, std::vector<T*>& comps_out)> collectCompsRecursively;
		collectCompsRecursively = [&](WorldComponent* c, std::vector<T*>& comps_out)
		{
			if (dynamic_cast<T*>(c))
				comps_out.push_back((T*)c);

			for (auto& child : c->GetChilds())
				collectCompsRecursively(child, comps_out);
		};

		std::vector<T*> comps;
		collectCompsRecursively(rootComp, comps);

		return comps;
	}

	__inline WorldComponent* GetRootComp() { return rootComp; }
	__inline std::vector<Behavior*>& GetBehaviors() { return behaviors; }

	__inline const std::function<void(float deltaSeconds)>& GetOnUpdate() { return onUpdate; }
	__inline const std::function<void(const rCollision& col)>& GetOnCollision() { return onCollision; }
	__inline const std::function<void(const rCollision& col)>& GetOnCollisionEnter() { return onCollisionEnter; }
	__inline const std::function<void(const rCollision& col)>& GetOnCollisionExit() { return onCollisionExit; }

	__inline const std::string& GetName() { return name; }
	__inline bool IsKilled() { return bKilled; }

	__inline std::vector<Actor*>& GetChilds() { return childs; }
	__inline Actor* GetParent() { return parent; }

protected:
	std::string name;

	WorldComponent* rootComp;

	Actor* parent;
	std::vector<Actor*> childs;

	// Behaviors -> ActorScripts
	std::vector<Behavior*> behaviors;

	std::function<void(float deltaSeconds)> onUpdate;
	std::function<void(const rCollision& col)> onCollision;
	std::function<void(const rCollision& col)> onCollisionEnter;
	std::function<void(const rCollision& col)> onCollisionExit;

	// Lifecycle
	bool bKilled;
};