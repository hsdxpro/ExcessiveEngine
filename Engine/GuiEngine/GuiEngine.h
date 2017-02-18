#pragma once
#include "GuiCanvas.h"

class GuiEngine
{
public:
	GuiEngine(IGraphicsEngine* graphicsEngine);
	~GuiEngine();

	GuiCanvas* AddCanvas();

	void Update(float deltaTime);

protected:
	IGraphicsEngine* graphicsEngine;

	//std::unordered_set<std::wstring, GuiCanvas> canvases;
	std::vector<GuiCanvas*> canvases;
};

inline GuiEngine::GuiEngine(IGraphicsEngine* graphicsEngine)
:graphicsEngine(graphicsEngine)
{
}

inline GuiEngine::~GuiEngine()
{
	for(auto& canvas : canvases)
		delete canvas;

	canvases.clear();
}

inline GuiCanvas* GuiEngine::AddCanvas()
{
	GuiCanvas* canvas = new GuiCanvas();
	canvases.push_back(canvas);
	return canvas;
}

inline void GuiEngine::Update(float deltaTime)
{
	for(GuiCanvas* canvas : canvases)
	{

	}
}