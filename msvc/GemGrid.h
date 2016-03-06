#pragma once
#include <iostream>
#include <random>
#include <tuple>

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
	bool IsCascadePresent();
	int DestroyGems();
	std::vector<std::pair<int, int>> GetGemsToDestroy();

	void ActivateGravity(std::vector<std::pair<int, int>> gemsToDestroy);
	bool GravityPull(std::vector<std::pair<int, int>> gemsToDestroy);

	bool IsGridLocked();
	void LockGrid();
	void UnlockGrid();

	// Constants
	const float gridXStart = 330.0f;
	const float gridYStart = 100.0f;
	const float gridOffset = 43.0f;
private:
	gemGrid8x8 gemGrid;
	std::vector<std::pair<int,int>> pullValues;
	std::vector<std::pair<int, int>> gemsToDestroy;

	// Pointer to engine
	King::Engine* engine;

	bool gridLocked;

	// Private functions
	bool IsInCascadeInitial(int x, int y, King::Engine::Texture color);
	void MarkToDestroy();
};

