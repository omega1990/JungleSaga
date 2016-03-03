#pragma once
#include <iostream>
#include <random>

#include <king\Engine.h>

class GemGrid
{
public:
	GemGrid();
	~GemGrid();

	void InitializeGrid();
	void PrintGrid();


	int** getGemGrid();


	// Constants
	const float gridXStart = 330.0f;
	const float gridYStart = 100.0f;
	const float gridOffset = 43.0f;
private:
	int** gemGrid;

	// Pointer to engine
	King::Engine* engine;



	// Private functions
	bool IsInCascadeInitial(int x, int y);
};

