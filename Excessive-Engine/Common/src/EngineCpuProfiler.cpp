#include "EngineCpuProfiler.h"
#include "Sys.h"

EngineCpuProfiler* EngineCpuProfiler::instance = nullptr;

EngineCpuProfiler::EngineCpuProfiler()
{
	window.create(sf::VideoMode(400, 400), "Engine - CpuProfiler");
	window.setPosition({ 0, 0 });

	assert(fontArial.loadFromFile(Sys::getWorkDir() + sf::String("arial.ttf")));
}

void EngineCpuProfiler::_internalupdateAndPresent()
{
	sf::Event evt;
	while (window.pollEvent(evt))
	{
		if (evt.type == sf::Event::Closed)
			window.close();
	}

	if (window.isOpen())
	{
		window.resetGLStates();
		window.clear(sf::Color::Black);

		sf::Text text;
		sf::String str("PROFILER...");

		text.setString(str);
		text.setFont(fontArial);
		text.setCharacterSize(30);
		text.setStyle(sf::Text::Bold);
		text.setColor(sf::Color::White);
		text.setPosition(100, 100);

		window.draw(text);

		window.display();
	}
}

void EngineCpuProfiler::updateAndPresent()
{
	if (!instance)
		instance = new EngineCpuProfiler();

	instance->_internalupdateAndPresent();
}