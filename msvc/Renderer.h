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

private:
	King::Engine* mEngine;
	GemGrid* grid;
};

