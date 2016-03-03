#pragma once
#include "GemGrid.h"
#include <king\Engine.h>

class Renderer
{
public:
	Renderer(King::Engine& mEngine, GemGrid& passedGrid);
	

	~Renderer();

	void RenderBackground();
	void RenderGemGrid();
	void RenderGemGrid(int x, int y, float offsetX, float offsetY);

private:
	King::Engine* mEngine;
	GemGrid* grid;
};

