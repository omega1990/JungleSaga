#pragma once
#include "GemGrid.h"
#include <king\Engine.h>

class Renderer
{
public:
	Renderer(King::Engine& mEngine, GemGrid& passedGrid);
	~Renderer();

	enum direction
	{
		HORIZONTAL_RIGHT,
		HORIZONTAL_LEFT,
		VERTICAL_UP,
		VERTICAL_DOWN,
		STATIONARY,
		OUT_OF_BOUNDS
	};

	void RenderBackground();
	void RenderGemGrid();
	void RenderGemGrid(int x, int y, float offsetX, float offsetY, direction dir);

private:
	King::Engine* mEngine;
	GemGrid* grid;
};

