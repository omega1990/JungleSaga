#pragma once
#include <king\Updater.h>
#include "GemGrid.h"
#include "Renderer.h"
#include "Gem.h"
#include <string>

class Game : public King::Updater 
{
public:
	Game();
	~Game();

	void Start();
	void Update();
	
	bool CheckGemInteraction();


private:
	int score;

	King::Engine mEngine;
	GemGrid* grid;
	gemGrid8x8& gridArray;
	Renderer* renderer;

	float mRotation;
	float mYellowDiamondX;
	float mYellowDiamondY;
	bool gemLocked = false;
	
	int selectedGemX;
	int selectedGemY;

	float angle;

	int switchGemX = 0;
	int switchGemY = 0;

	Gem::direction gemDirection;

	Gem* clickedGem = nullptr;
	Gem* swithingGem = nullptr;

	Gem::direction GetMouseDirection(float mouseStartPositionX, float mouseStartPositionY);
	bool Game::IsClickInsideGameArea();
	std::pair<float, Gem::direction> GetGemOffset(int mouseX, int mouseY);
};
