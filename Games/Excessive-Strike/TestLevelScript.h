#pragma once
#include "Core\Script.h"
#include "PlayerScript.h"

class TestLevelScript : public Script
{
public:
	TestLevelScript();

	void Update(float deltaSeconds);
protected:

	PlayerScript playerScript;
};