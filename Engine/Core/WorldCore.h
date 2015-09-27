#pragma once
#include "EngineCore.h"

class Script;
class EngineWorld
{
public:
	template<class ScriptClass>
	__inline Script* AddScript() { return Core.AddScript<ScriptClass>();}

	__inline Actor* SpawnActor(WorldComponent* rootComp = new Transform3DComponent()) { return Core.SpawnActor(rootComp); }
	__inline Actor* SpawnActor_MeshFromFile(const std::string& modelFilePath) { return Core.SpawnActor_MeshFromFile(modelFilePath); }
	__inline Actor* SpawnActor_RigidBodyFromFile(const std::string& modelFilePath, float mass) { return Core.SpawnActor_RigidBodyFromFile(modelFilePath, mass); }
	__inline Actor* SpawnActor_RigidBodyCapsule(float height, float radius, float mass = 0) { return Core.SpawnActor_RigidBodyCapsule(height, radius, mass); }
	__inline Actor* SpawnActor_Camera() { return Core.SpawnActor_Camera(); }

	__inline MeshComponent*			SpawnComp_MeshFromFile(const std::string& modelFilePath) { return Core.SpawnComp_MeshFromFile(modelFilePath); }
	__inline RigidBodyComponent*	SpawnComp_RigidBodyFromFile(const std::string& modelFilePath, float mass) { return Core.SpawnComp_RigidBodyFromFile(modelFilePath, mass); }
	__inline RigidBodyComponent*	SpawnComp_RigidBodyCapsule(float height, float radius, float mass = 0) { return Core.SpawnComp_RigidBodyCapsule(height, radius, mass); }
	__inline CameraComponent*		SpawnComp_Camera() { return Core.SpawnComp_Camera(); }
	__inline Transform3DComponent*	SpawnComp_Transform3D() { return Core.SpawnComp_Transform3D(); }

	
	__inline void Destroy(Actor* a) { return Core.Destroy(a); }
	__inline void Destroy(WorldComponent* c) { return Core.Destroy(c); }

	__inline bool TraceClosestPoint_Physics(const mm::vec3& from, const mm::vec3& to, rPhysicsTraceResult& traceInfo_out)
	{
		Core.TraceClosestPoint_Physics(from, to, traceInfo_out);
	}
};

extern EngineWorld World;