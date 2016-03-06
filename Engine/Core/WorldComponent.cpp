#include "MeshComponent.h"
#include "RigidBodyComponent.h"
#include "SoftBodyComponent.h"
#include "CameraComponent.h"
#include "GameWorld.h"

CameraComponent* WorldComponent::AddComponent_Camera()
{
	CameraComponent* c = World.AddComponent_Camera();
	childs.push_back(c);

	return c;
}

MeshComponent* WorldComponent::AddComponent_Mesh(const std::string& modelPath)
{
	MeshComponent* c = World.AddComponent_Mesh(modelPath);
	childs.push_back(c);

	return c;
}