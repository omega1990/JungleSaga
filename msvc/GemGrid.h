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
	gemGrid8x8& GetGemGrid();
	King::Engine::Texture GemGrid::GenerateRandomGemColor() const;	
	bool IsCascadePresent();
	int DestroyGems();
	std::vector<std::pair<int, int>> GetGemsToDestroy();
	std::vector<std::pair<int, int>> GetColumnOffsets();
	int GetColumnOffset(int column);
	
	void ActivateGravity();
	void GravityPull();
	bool IsGravityActive();

	bool IsGridLocked();
	void LockGrid();
	void UnlockGrid();

	void PrintGrid();

	void MoveGem();
	void TriggerGemMoving(int fromX, int fromY, int toX, int toY);

	bool gemMoving = false;
	
	int fromX;
	int fromY;
	int toX;
	int toY;

	// Constants
	const float gridXStart = 330.0f;
	const float gridYStart = 100.0f;
	const float gridOffset = 43.0f;

private:
	gemGrid8x8 gemGrid;
	std::vector<std::pair<int, int>> gemsToDestroy;
	std::vector<std::pair<int, int>> columnOffsets;	
	King::Engine* engine;

	bool gridLocked;
	int gravity;
	
	bool isInCascadeInitial(int x, int y, King::Engine::Texture color);
	void markToDestroy();
	bool destructmentInProgress();
	void findMatches();
	void findMatchesHorizontal();
	void findMatchesVertical();
	bool alreadyMarkedForDestruction(int column, int row);
};

