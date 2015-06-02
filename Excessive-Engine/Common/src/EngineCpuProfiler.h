#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Font.hpp"

#include <assert.h>

class EngineCpuProfiler
{
protected:
	EngineCpuProfiler();
	void _internalupdateAndPresent();

public:
	static void updateAndPresent();

protected:
	sf::RenderWindow window;
	sf::Font fontArial;
	static EngineCpuProfiler* instance;
};