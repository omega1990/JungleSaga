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

	enum GameMode
	{
		MENU, 
		MENUSLIDE,
		GAME
	};

	GameMode mode = MENU;

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

	int clickDown = 0;
	bool clickUp = false;;

	float angle;

	int switchGemX = 0;
	int switchGemY = 0;

	Gem::direction gemDirection;

	Gem* clickedGem = nullptr;
	Gem* switchingGem = nullptr;
	Gem* previousSwitchingGem = nullptr;

	Gem::direction getMouseDirection(float mouseStartPositionX, float mouseStartPositionY);
	bool Game::isClickInsideGameArea();
	std::pair<float, Gem::direction> getGemOffset(Gem &gem);
	void Game::handleGemClick();
	void Game::handleGemSwipe();
	void Game::resetClickedGemCoordinates();

	const int gameDuration = 60;


	float slidePositionX = 0.0f;
	float slidePositionY = 0.0f;
	float slideIncrementer = 0.0f;
};
