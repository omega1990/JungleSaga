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
	
	bool HandleGemInteraction();
private:
	int score;

	King::Engine mEngine;
	GemGrid* grid;
	gemGrid8x8& gridArray;
	Renderer* renderer;

	float mRotation;
	float referenceClock = 0.0f;
	bool gemLocked = false;
	bool swipePerformed = false;
	bool firstGemLocked = false;
	bool clickReleased = false;

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
