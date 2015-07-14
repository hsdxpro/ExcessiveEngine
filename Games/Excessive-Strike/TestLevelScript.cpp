#include "TestLevelScript.h"
#include "Core\Core.h"
#include "PlayerScript.h"


TestLevelScript::TestLevelScript()
{
	// Add ground & sky to level
	auto& groundModelPath = Sys::GetWorkDir() + L"Assets/demo_ground.dae";
	auto& skyModelPath = Sys::GetWorkDir() + L"Assets/skybox.dae";

	gCore.SpawnCompRigidBodyFromFile(groundModelPath, 0);
	gCore.SpawnCompGraphicsFromFile(groundModelPath);
	gCore.SpawnCompGraphicsFromFile(skyModelPath)->SetScaleLocal({ 1000, 1000, 1000 });

	gCore.SpawnThing(gCore.AddActorScript<PlayerScript>());
}

void TestLevelScript::Update(float deltaSeconds)
{

}