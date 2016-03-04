#pragma once
#include <iostream>
#include <random>

#include <king\Engine.h>
#include "Gem.h"
#include "Common.h"

class GemGrid
{
public:
	GemGrid();
	~GemGrid();

	void InitializeGrid();
	void PrintGrid();

	gemGrid8x8& getGemGrid();
	King::Engine::Texture GemGrid::GenerateRandomTexture() const;

	// Constants
	const float gridXStart = 330.0f;
	const float gridYStart = 100.0f;
	const float gridOffset = 43.0f;
private:
	gemGrid8x8 gemGrid;

	// Pointer to engine
	King::Engine* engine;

	// Private functions
	bool IsInCascadeInitial(int x, int y, King::Engine::Texture color);
};

