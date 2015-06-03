#include "EngineCpuProfiler.h"
#include "Sys.h"
#include "Factory.h"
#include "ITimer.h"
#include <sstream>
#include <iomanip>
#include "GL\glew.h"

EngineCpuProfiler* EngineCpuProfiler::instance = nullptr;
size_t EngineCpuProfiler::sectionIDGenerator = 0;

// "Profiler" section
EngineCpuProfiler::Section::Section(const std::string& name)
:name(name)
{
	timer = Factory::createTimer();

	// Add that Section to tree
	sectionNode* node = new sectionNode();
		node->name = name;
		node->ID = sectionIDGenerator++;
		node->bLiving = true;
		node->parent = nullptr;
	EngineCpuProfiler::GetSingletonInstance()->sectionTree.push_back(node);

	ID = node->ID;

	timer->start();
}

EngineCpuProfiler::Section::~Section()
{
	size_t profiledMicroSeconds = timer->getMicroSecondsPassed();

	auto sectionNodes = EngineCpuProfiler::GetSingletonInstance()->sectionTree;
	sectionNode* us = nullptr;
	for (int32_t i = (int32_t)sectionNodes.size() - 1; i >= 0; i--)
	{
		assert(i >= 0);

		if (sectionNodes[i]->ID == ID) // Search ourselves
		{
			us = sectionNodes[i];
			us->bLiving = false;
			us->profiledMicroSeconds = profiledMicroSeconds;

			if (i == 0) // We are super parent (roots)
				return;
		}

		if (sectionNodes[i]->ID != ID && sectionNodes[i]->bLiving) // Search parent... -> Living so between a construction and destruction...
		{
			assert(us);
			sectionNodes[i]->childs.push_back(us);
			us->parent = sectionNodes[i];
			break;
		}
	}

	delete timer;
}

EngineCpuProfiler::EngineCpuProfiler()
{
	window.create(sf::VideoMode(600, 600), "Engine - CpuProfiler");
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
		window.setActive(true);

		// Unbind everything from opengl
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(0, 0);
		
		glBindVertexBuffers(0, 16, nullptr, nullptr, nullptr);
		
		for (uint8_t i = 0; i < 16; i++)
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, i, 0);
			glBindVertexBuffer(i, 0, 0, 0);
		}
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glVertexPointer(0, GL_FLOAT, 0, 0);

		window.resetGLStates();
		window.clear(sf::Color::Black);	

		// Search for lowest perf node
		size_t lowestPerfMicroSec = 0;
		lowestPerfSectionNode = nullptr;
		for (auto& a : sectionTree)
		{
			if (lowestPerfMicroSec <= a->profiledMicroSeconds)
			{
				lowestPerfMicroSec = a->profiledMicroSeconds;
				lowestPerfSectionNode = a;
			}
		}

		size_t curNodePosY = 0;
		for (auto& a : sectionTree)
			if (a->parent == nullptr) // root Component
				_internalDrawSectionTreeRecursively(a, curNodePosY, 0);
		
		window.display();
		window.setActive(false);
	}

	for (auto& a : sectionTree)
		delete a;

	sectionTree.clear();
}

void EngineCpuProfiler::_internalDrawSectionTreeRecursively(sectionNode* node, size_t& curNodePosY_inout, size_t depth)
{
	// Draw that node...
	sf::Text text;
	std::stringstream ss;

	ss.str("");
	ss << std::setprecision(2);
	ss << std::fixed;
	ss << (double)node->profiledMicroSeconds / 1000.0;

	std::string printThat = "- " + node->name + ": " + ss.str() + " ms";
	sf::String str(printThat.c_str());

	text.setString(str);
	text.setFont(fontArial);
	text.setCharacterSize(20);

	float interp = 0.f;
	
	if (lowestPerfSectionNode->profiledMicroSeconds != 0)
		interp = (float)((double)node->profiledMicroSeconds / lowestPerfSectionNode->profiledMicroSeconds);

	sf::Color hybridColor;
		hybridColor.r = round(255.f * interp);
		hybridColor.g = round(255.f * (1 - interp));
		hybridColor.b = 0;
		hybridColor.a = 255.f;
	text.setColor(hybridColor);

	text.setPosition(20 + 50 * depth, 20 + curNodePosY_inout);
	window.draw(text);

	curNodePosY_inout += 30;

	for (auto& n : node->childs)
		_internalDrawSectionTreeRecursively(n, curNodePosY_inout, depth + 1);
}

void EngineCpuProfiler::updateAndPresent()
{
	if (!EngineCpuProfiler::instance)
		EngineCpuProfiler::instance = new EngineCpuProfiler();

	EngineCpuProfiler::instance->_internalupdateAndPresent();
}

EngineCpuProfiler* EngineCpuProfiler::GetSingletonInstance()
{
	if (!EngineCpuProfiler::instance)
		EngineCpuProfiler::instance = new EngineCpuProfiler();
	return EngineCpuProfiler::instance;
}