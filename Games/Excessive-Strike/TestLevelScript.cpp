#include "TestLevelScript.h"
#include "Core\Core.h"
#include "PlayerScript.h"
#include "ExcessiveStrikeCommon.h"


TestLevelScript::TestLevelScript()
{
	// Add ground to game
	auto& groundModelPath = L"Assets/demo_ground.dae";
	gCore->SpawnComp_RigidBodyFromFile(groundModelPath, 0)->SetCollisionGroup(eES_CollisionGroup::GROUND);
	gCore->SpawnComp_MeshFromFile(groundModelPath);

	// Add sky to game
	gCore->SpawnComp_MeshFromFile(L"Assets/skybox.dae")->SetScaleLocal({ 1000, 1000, 1000 });
}

void TestLevelScript::Update(float deltaSeconds)
{
	playerScript.Update(deltaSeconds);
}