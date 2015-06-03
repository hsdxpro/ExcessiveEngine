#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Font.hpp"

#include <assert.h>
#include <string>

#ifdef DEBUG_BUILD
	#define PROFILE_ENGINE
#endif

#ifdef PROFILE_ENGINE
	#define CONCAT_MACRO(x, y) x##y
	#define CONCAT_MACTO2(x,y) CONCAT_MACRO(x,y)
	#define PROFILER(x) EngineCpuProfiler::Section CONCAT_MACTO2(section_, __COUNTER__)(x)
#else
	#define PROFILER(x) (x)
#endif

class ITimer;

class EngineCpuProfiler
{
public:
	class Section
	{
	public:
		Section(const std::string& name);
		~Section();

	protected:
		ITimer* timer;
		std::string name;
		size_t ID;
	};

	struct sectionNode
	{
		std::string name;
		size_t ID;

		size_t	profiledMicroSeconds;
		bool	bLiving;

		sectionNode* parent;
		std::vector<sectionNode*> childs;
	};

protected:
	EngineCpuProfiler();
	void _internalupdateAndPresent();
	void _internalDrawSectionTreeRecursively(sectionNode* node, size_t& curNodePosY_inout, size_t depth);

public:
	static void updateAndPresent();
	static EngineCpuProfiler* GetSingletonInstance();

protected:
	static EngineCpuProfiler* instance;
	static size_t sectionIDGenerator;

	sf::RenderWindow window;
	sf::Font fontArial;

public:
	// Section tree
	std::vector<sectionNode*> sectionTree;

	sectionNode* lowestPerfSectionNode;
};