#pragma once
#include "GemGrid.h"
#include "Gem.h"
#include <king\Engine.h>

class Renderer
{
public:
	Renderer(King::Engine& mEngine, GemGrid& passedGrid);
	~Renderer();

	void RenderBackground();
	void RenderGemGrid();
	void RenderToBeDestroyed(std::vector<std::pair<int, int>> gemsToBeDestroyed);
	void RenderTop();


private:
	King::Engine* mEngine;
	GemGrid* grid;
};

