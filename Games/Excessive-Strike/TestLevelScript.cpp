#include "TestLevelScript.h"
#include "Core\EngineCore.h"
#include "PlayerScript.h"
#include "ExcessiveStrikeCommon.h"
#include "mymath\mymath.h"


TestLevelScript::TestLevelScript()
{
	// Add ground to game
	auto groundModelPath = "Assets/Terminal/terminal_blender.dae";
	//Assets/demo_ground.dae
	auto groundRigidActor = World.SpawnActor_RigidBodyFromFile(groundModelPath, 0);
	groundRigidActor->SetCollisionGroup(eES_CollisionGroup::GROUND);
	groundRigidActor->SetName("ground");
	groundRigidActor->Attach(World.SpawnComp_MeshFromFile(groundModelPath));
	//groundRigidActor->Rot(mm::quat(3.14159265 / 2, mm::vec3(1, 0, 0)));
	
	// Add sky to game
	auto sky = World.SpawnComp_MeshFromFile("Assets/skybox.dae");
	sky->SetScaleLocal({ 1000, 1000, 1000 });
	//sky->Rot(mm::quat(3.14159265 / 2, mm::vec3(1, 0, 0)));

	// Set up collision layers..
	Physics.SetLayerCollision(eES_CollisionGroup::PLAYER, eES_CollisionGroup::SHELL, false);
	Physics.SetLayerCollision(eES_CollisionGroup::GROUND, eES_CollisionGroup::GROUND, false);
	Physics.SetLayerCollision(eES_CollisionGroup::SHELL, eES_CollisionGroup::SHELL, false);
	Physics.SetLayerCollision(eES_CollisionGroup::SHELL, eES_CollisionGroup::PLAYER, false);
	Physics.SetLayerCollision(eES_CollisionGroup::BULLET, eES_CollisionGroup::SHELL, false);
}

void TestLevelScript::Update(float deltaSeconds)
{
	playerScript.Update(deltaSeconds);
}