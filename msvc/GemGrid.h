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

	void InitializeGrid(bool reshuffle = false);
	gemGrid8x8& GetGemGrid();
	King::Engine::Texture GemGrid::GenerateRandomGemColor() const;	
	bool IsCascadePresent();
	int DestroyGems();
	std::vector<std::pair<int, int>> GetGemsToDestroy();
	std::vector<std::pair<int, float>> GetColumnOffsets();
	float GetColumnOffset(int column);

	void EmptyGrid();
	void Reshuffle();
	
	void ActivateGravity();
	void GravityPull();
	bool IsGravityActive();

	bool IsGridLocked();
	void LockGrid();
	void UnlockGrid();

	void PrintGrid();

	void SwitchGems(int fromX, int fromY, int toX, int toY);
	void AnimateGemSwitch();
	void SwitchGemsPositions();

	possibleMoves FindPossibleMoves();

	bool gemMoving = false;
	bool checkPossibleMoves = true;


	// Constants
	const float gridXStart = 330.0f;
	const float gridYStart = 100.0f;
	const float gridOffset = 43.0f;

private:
	gemGrid8x8 gemGrid;
	std::vector<std::pair<int, int>> gemsToDestroy;

	// first: from j | second: offset 
	std::vector<std::pair<int, float>> columnOffsets;	
	King::Engine* engine;
	possibleMoves possibleMoves;

	bool gridLocked;

	float gravityIncrementer = 0.0f;
	const float gravity = 1.0f;




	//stavi getter i setter
	bool switchBack = false;

	int fromX;
	int fromY;
	int toX;
	int toY;
	
	bool isInCascadeInitial(int x, int y, King::Engine::Texture color);
	void markToDestroy();
	bool destructionPending();
	bool findMatches(bool check);
	bool findMatchesHorizontal(bool check);
	bool findMatchesVertical(bool check);
	bool alreadyMarkedForDestruction(int column, int row);
	void switchAndCheckMatches(int x1, int y1, int x2, int y2);

};

