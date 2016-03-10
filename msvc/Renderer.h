#pragma once
#include "GemGrid.h"
#include "Gem.h"
#include <king\Engine.h>

class Renderer
{
public:
	Renderer(King::Engine& mEngine, GemGrid& passedGrid);
	~Renderer();

	void RenderStartScreen(float x = 0.0f, float y = 0.0f);
	void RenderBackground();
	void RenderGemGrid();
	void RenderToBeDestroyed(std::vector<std::pair<int, int>> gemsToBeDestroyed);
	void RenderTop();
	void RenderMenu();
	void RenderMenuSlide(float x = 0.0f, float y = 0.0f);
	void Renderer::RenderGameOverSlide(float x = 0.0f, float y = 0.0f);
	void RenderGameOver(const char* score);



	void RenderSelected(int selectedGemX, int selectedGemY);

	bool MatchRendererInProgress = false;

private:
	King::Engine* mEngine;
	GemGrid* grid;


	float clock = 0.0f;
	
};

