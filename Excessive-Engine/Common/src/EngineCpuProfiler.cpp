#include "EngineCpuProfiler.h"
#include "Sys.h"
#include "Factory.h"
#include "ITimer.h"
#include <sstream>
#include <iomanip>
#include "GL\glew.h"

EngineCpuProfiler* EngineCpuProfiler::instance = nullptr;
EngineCpuProfiler::ProfilerNode* EngineCpuProfiler::lastConstructedTreeNode = nullptr;
size_t EngineCpuProfiler::IDGenerator = 0;

// Scope profiling
EngineCpuProfiler::Scope::Scope(const std::string& name)
:name(name)
{
	IDGenerator++;

	timer = Factory::createTimer();
	
	// Add that Section to tree
	ProfilerNode* node = new ProfilerNode();
		node->name = name;
	
	// This is a root node !
	if (lastConstructedTreeNode == nullptr)
	{
		node->parent = nullptr;
		EngineCpuProfiler::GetSingletonInstance()->treeRootComponents.push_back(node);
	}
	else // This is a child node
	{
		node->parent = lastConstructedTreeNode;
		lastConstructedTreeNode->childs.push_back(node);
	}
	
	lastConstructedTreeNode = node;

	timer->start();
}

EngineCpuProfiler::Scope::~Scope()
{
	// Save profiled time
	lastConstructedTreeNode->profiledSeconds = timer->getSecondsPassed();
	
	// Go up 1 on tree
	lastConstructedTreeNode = lastConstructedTreeNode->parent;
	
	delete timer;
}

// Scope sum profiling
EngineCpuProfiler::ScopeSum::LifeCycleHelper::LifeCycleHelper(ScopeSum* scopeSumProfiler)
:scopeSumProfiler(scopeSumProfiler)
{
	auto& treeRootComponents = EngineCpuProfiler::GetSingletonInstance()->treeRootComponents;
	if (!lastConstructedTreeNode)
	{
		scopeSumProfiler->profilerNode->parent = nullptr;
		EngineCpuProfiler::GetSingletonInstance()->treeRootComponents.push_back(scopeSumProfiler->profilerNode);
	}
	else if (scopeSumProfiler->ID == IDGenerator++)
	{
		// scopeSumProfiler static who made "new profilerNode()", so we need to clear it
		scopeSumProfiler->profilerNode->profiledSeconds = 0;
		scopeSumProfiler->profilerNode->childs.clear();
		scopeSumProfiler->profilerNode->parent = nullptr;

		scopeSumProfiler->profilerNode->parent = lastConstructedTreeNode;
		lastConstructedTreeNode->childs.push_back(scopeSumProfiler->profilerNode);
	}

	lastConstructedTreeNode = scopeSumProfiler->profilerNode;

	scopeSumProfiler->timer->reset();
}

EngineCpuProfiler::ScopeSum::LifeCycleHelper::~LifeCycleHelper()
{
	scopeSumProfiler->profilerNode->profiledSeconds += scopeSumProfiler->timer->getSecondsPassed();

	// Go up 1 on tree
	lastConstructedTreeNode = lastConstructedTreeNode->parent;
}

EngineCpuProfiler::ScopeSum::ScopeSum(const std::string& name)
:name(name)
{
	// Itt a baj, DLL - nél IDGenerator 0 ad vissza, static lib - nél meg 3 - at
	ID = IDGenerator;

	timer = Factory::createTimer();

	profilerNode = new ProfilerNode();
		profilerNode->name = name;
}

EngineCpuProfiler::ScopeSum::~ScopeSum()
{

}

EngineCpuProfiler::EngineCpuProfiler()
{
	window.create(sf::VideoMode(600, 600), "Engine - CpuProfiler");
	window.setPosition({ 0, 0 });

	bool b = fontArial.loadFromFile(Sys::getWorkDir() + sf::String("arial.ttf"));
	assert(b);
}

void EngineCpuProfiler::_internalupdateAndPresent()
{
	IDGenerator = 0;

	sf::Event evt;
	while (window.pollEvent(evt))
	{
		if (evt.type == sf::Event::Closed)
			window.close();
		else if (evt.type == sf::Event::Resized)
		{
			sf::View view;
				view.setCenter(sf::Vector2f((float)evt.size.width * 0.5, (float)evt.size.height * 0.5));
				view.setSize(sf::Vector2f((float)evt.size.width, (float)evt.size.height));
				view.setViewport(sf::FloatRect(0, 0, 1, 1));
			window.setView(view);
		}
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
		double lowestPerfSec = 0;
		lowestPerfSectionNode = nullptr;
		for (auto& a : treeRootComponents)
		{
			if (lowestPerfSec <= a->profiledSeconds)
			{
				lowestPerfSec = a->profiledSeconds;
				lowestPerfSectionNode = a;
			}
		}

		size_t curNodePosY = 0;
		for (auto& a : treeRootComponents)
			_internalDrawSectionTreeRecursively(a, curNodePosY, 0);
		
		window.display();
		window.setActive(false);
	}

	for (auto& a : treeRootComponents)
		delete a;

	treeRootComponents.clear();
}

void EngineCpuProfiler::_internalDrawSectionTreeRecursively(ProfilerNode* node, size_t& curNodePosY_inout, size_t depth)
{
	// Draw that node...
	sf::Text text;
	std::stringstream ss;

	ss.str("");
	ss << std::setprecision(2);
	ss << std::fixed;
	ss << (double)node->profiledSeconds * 1000;

	std::string printThat = "- " + node->name + ": " + ss.str() + " ms";
	sf::String str(printThat.c_str());

	text.setString(str);
	text.setFont(fontArial);
	text.setCharacterSize(20);

	float interp = 0.f;
	
	if (lowestPerfSectionNode->profiledSeconds != 0)
		interp = (float)((double)node->profiledSeconds / lowestPerfSectionNode->profiledSeconds);

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
	GetSingletonInstance();
	instance->_internalupdateAndPresent();
}

EngineCpuProfiler* EngineCpuProfiler::GetSingletonInstance()
{
	if (!instance)
		instance = new EngineCpuProfiler();

	return instance;
}