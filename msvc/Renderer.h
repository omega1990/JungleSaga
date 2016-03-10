#pragma once
#include "GemGrid.h"
#include "Gem.h"
#include <king\Engine.h>

class Renderer
{
public:
	Renderer(King::Engine& mEngine, GemGrid& passedGrid);
	~Renderer();

	void RenderStartScreen(float x = 0.0f, float y = 0.0f) const;
	void RenderBackground() const;
	void RenderGemGrid() const;
	void RenderToBeDestroyed(std::vector<std::pair<int, int>> gemsToBeDestroyed);
	void RenderTop() const;
	void RenderMenu() const;
	void RenderMenuSlide(float x = 0.0f, float y = 0.0f) const;
	void Renderer::RenderGameOverSlide(float x = 0.0f, float y = 0.0f) const;
	void RenderGameOver(const char* score) const;
	void RenderScore(const char* score) const;
	void RenderTime(const char* time) const;

	void RenderSelected(int selectedGemX, int selectedGemY) const;
	
	bool IsMatchRenderingInProgress() const;
private:
	King::Engine* mEngine;
	GemGrid* grid;
	
	bool matchRendererInProgress = false;

	float clock;
};

