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


	//gCore.SpawnThing(new ActorScript(new Actor()));

	
	gCore.SpawnThing(gCore.AddActorScript<PlayerScript>());
	
	//gCore.SpawnThing(gCore.AddActorBehavior(gCore.AddActorScript<PlayerScript>()));
	

	// Szeretnék egy Thing - et létrehozni 1 darab ActorScript - bõl

	// It can be done easier, but we need to test features
	//Thing* player = gCore.SpawnThing(new PlayerScript(new Actor()));
	//Thing* player = gCore.SpawnThing();
	//	player->AddBehavior(new ActorBehavior(new PlayerScript());
	//	player->SetActor(new Actor());
}

void TestLevelScript::Update(float deltaSeconds)
{

}