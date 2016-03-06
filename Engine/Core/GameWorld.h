#pragma once
#include "EngineCore.h"
#include "MeshComponent.h"
#include "RigidBodyComponent.h"
#include "CameraComponent.h"
#include "Transform3DComponent.h"
#include "WorldComponent.h"
//#include "Script.h"

class Script;

class GameWorld
{
public:
	template<class ScriptClass>
	inline Script* AddScript();

	inline Actor* AddActor();
	//inline Actor* AddActor(WorldComponent* rootComp);
	inline Actor* AddActor(const std::string& modelFilePath, float mass = 0);
	inline Actor* AddActor_Mesh(const std::string& modelFilePath);
	inline Actor* AddActor_RigidBody(const std::string& modelFilePath, float mass = 0);
	inline Actor* AddActor_RigidBodyCapsule(float height, float radius, float mass = 0);
	inline Actor* AddActor_Camera();

	inline MeshComponent*		 AddComponent_Mesh(const std::string& modelFilePath);
	inline RigidBodyComponent*	 AddComponent_RigidBody(const std::string& modelFilePath, float mass = 0);
	inline RigidBodyComponent*	 AddComponent_RigidBodyCapsule(float height, float radius, float mass = 0);
	inline CameraComponent*		 AddComponent_Camera();
	inline Transform3DComponent* AddComponent_Transform3D();
	
	inline void Remove(Actor* a);
	inline void Remove(WorldComponent* c);

	inline void SetCam(CameraComponent* c);

	inline bool TraceClosestPoint_Physics(const mm::vec3& from, const mm::vec3& to, PhysicsTraceResult& traceInfo_out);
};
extern GameWorld World;

template<class ScriptClass>
Script* GameWorld::AddScript() 
{ 
	return Core.AddScript<ScriptClass>(); 
}

Actor* GameWorld::AddActor() 
{ 
	return Core.AddActor();
}

Actor* GameWorld::AddActor_Mesh(const std::string& modelFilePath) 
{ 
	return Core.AddActor_Mesh(modelFilePath);
}

Actor* GameWorld::AddActor(const std::string& modelFilePath, float mass /*= 0*/)
{
	return Core.AddActor(modelFilePath, mass);
}

Actor* GameWorld::AddActor_RigidBody(const std::string& modelFilePath, float mass /*= 0*/)
{
	return Core.AddActor_RigidBody(modelFilePath, mass);
}

Actor* GameWorld::AddActor_RigidBodyCapsule(float height, float radius, float mass /*= 0*/)
{
	return Core.AddActor_RigidBodyCapsule(height, radius, mass);
}

Actor* GameWorld::AddActor_Camera()
{
	return Core.AddActor_Camera();
}

MeshComponent* GameWorld::AddComponent_Mesh(const std::string& modelFilePath)
{
	return Core.AddComponent_Mesh(modelFilePath);
}

RigidBodyComponent* GameWorld::AddComponent_RigidBody(const std::string& modelFilePath, float mass)
{
	return Core.AddComponent_RigidBody(modelFilePath, mass);
}

RigidBodyComponent* GameWorld::AddComponent_RigidBodyCapsule(float height, float radius, float mass /*= 0*/) 
{ 
	return Core.AddComponent_RigidBodyCapsule(height, radius, mass);
}
CameraComponent* GameWorld::AddComponent_Camera()
{
	return Core.AddComponent_Camera();
}

Transform3DComponent* GameWorld::AddComponent_Transform3D()
{
	return Core.AddComponent_Transform3D();
}

void GameWorld::Remove(Actor* a)
{
	return Core.Remove(a);
}

void GameWorld::Remove(WorldComponent* c)
{
	return Core.Remove(c);
}

void GameWorld::SetCam(CameraComponent* c)
{
	return Core.SetCam(c);
}

bool GameWorld::TraceClosestPoint_Physics(const mm::vec3& from, const mm::vec3& to, PhysicsTraceResult& traceInfo_out)
{
	Core.TraceClosestPoint_Physics(from, to, traceInfo_out);
}