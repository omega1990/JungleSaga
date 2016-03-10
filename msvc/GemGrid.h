#pragma once
#include <iostream>
#include <random>
#include <tuple>
#include <algorithm>
#include <king\Engine.h>

#include "Gem.h"
#include "Common.h"

class GemGrid
{
public:
	GemGrid();
	~GemGrid();

	void InitializeGrid(bool reshuffle = false);
	gemGrid8x8& GetGemGrid();
	King::Engine::Texture GemGrid::GenerateRandomGemColor() const;	
	bool IsCascadePresent();
	int DestroyGems();
	std::vector<std::pair<int, int>> GetGemsToDestroy() const;
	std::vector<std::pair<int, float>> GetColumnOffsets() const;
	float GetColumnOffset(int column) const;

	void EmptyGrid();
	void Reshuffle();
	
	void ActivateGravity();
	void GravityPull();
	bool IsGravityActive() const;

	bool IsGridLocked() const;
	void LockGrid();
	void UnlockGrid();

	void PrintGrid() const;

	void SwitchGems(int fromX, int fromY, int toX, int toY);
	void AnimateGemSwitch();
	void SwitchGemsPositions();

	possibleMoves FindPossibleMoves();

	bool IsGemMoving() const;
	void SetGemMoving(bool isMoving = true);

	bool possibleCheckPending() const;


	// Constants
	const float gridXStart = 330.0f;
	const float gridYStart = 100.0f;
	const float gridOffset = 43.0f;

private:
	gemGrid8x8 gemGrid;
	std::vector<std::pair<int, int>> gemsToDestroy;

	std::vector<std::pair<int, float>> columnOffsets;	
	King::Engine* engine;
	possibleMoves possibleMoves;

	bool gridLocked;

	float gravityIncrementer;
	const float gravity = 1.0f;

	int fromX, fromY, toX, toY;

	bool switchBack;

	bool gemMoving = false;
	bool checkPossibleMoves = true;
	
	bool isInCascadeInitial(int x, int y, King::Engine::Texture color);
	void markToDestroy();
	bool destructionPending();
	bool findMatches(bool check);
	bool findMatchesHorizontal(bool check);
	bool findMatchesVertical(bool check);
	bool alreadyMarkedForDestruction(int column, int row);
	void switchAndCheckMatches(int x1, int y1, int x2, int y2);
};

