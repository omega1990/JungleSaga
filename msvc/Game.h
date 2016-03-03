#pragma once
#include <king\Updater.h>
#include "GemGrid.h"
#include "Renderer.h"

class Game : public King::Updater 
{
public:
	Game(GemGrid& passedGrid);
	~Game();

	void Start();
	void Update();
	
	bool IsGemClicked();

	enum direction
	{
		HORIZONTAL,
		VERTICAL, 
		STATIONARY
	};

private:
	King::Engine mEngine;
	GemGrid* grid;
	Renderer* renderer;
	float mRotation;
	float mYellowDiamondX;
	float mYellowDiamondY;
	bool objectLocked = false;
	int** gridArray;
	int selectedGemX = -1;
	int selectedGemY = -1;

	float moseMoveStartX = 0.0f;
	float moseMoveStartY = 0.0f;
	float angle;

	direction gemDirection;
};
