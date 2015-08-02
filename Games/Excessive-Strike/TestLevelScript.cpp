#include "TestLevelScript.h"
#include "Core\Core.h"
#include "PlayerScript.h"
#include "ExcessiveStrikeCommon.h"


TestLevelScript::TestLevelScript()
{
	// Add ground to game
	auto groundModelPath = "Assets/Terminal/terminal_blender.dae";
	//Assets/demo_ground.dae
	auto groundRigidActor = gCore->SpawnActor_RigidBodyFromFile(groundModelPath, 0);
	groundRigidActor->SetCollisionGroup(eES_CollisionGroup::GROUND);
	groundRigidActor->SetName("ground");
	gCore->SpawnComp_MeshFromFile(groundModelPath);

	// Add sky to game
	gCore->SpawnComp_MeshFromFile("Assets/skybox.dae")->SetScaleLocal({ 1000, 1000, 1000 });

	// Set up collision layers..
	//gCore->SetLayerCollision(eES_CollisionGroup::GROUND, eES_CollisionGroup::SHELL, true);

	gCore->SetLayerCollision(eES_CollisionGroup::PLAYER, eES_CollisionGroup::SHELL, false);
	//gCore->SetLayerCollision(eES_CollisionGroup::PLAYER, eES_CollisionGroup::BULLET, false);

	gCore->SetLayerCollision(eES_CollisionGroup::GROUND, eES_CollisionGroup::GROUND, false);

	gCore->SetLayerCollision(eES_CollisionGroup::SHELL, eES_CollisionGroup::SHELL, false);
	gCore->SetLayerCollision(eES_CollisionGroup::SHELL, eES_CollisionGroup::PLAYER, false);

//	gCore->SetLayerCollision(eES_CollisionGroup::BULLET, eES_CollisionGroup::PLAYER, false);
	gCore->SetLayerCollision(eES_CollisionGroup::BULLET, eES_CollisionGroup::SHELL, false);
}

void TestLevelScript::Update(float deltaSeconds)
{
	playerScript.Update(deltaSeconds);
}