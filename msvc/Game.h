#pragma once
#include <SDL/SDL_mixer.h>
#include <king\Updater.h>
#include <string>
#include "GemGrid.h"
#include "Renderer.h"
#include "Gem.h"
#include "MusicMixer.h"

class Game : public King::Updater 
{
public:
	Game();
	~Game();

	void Start();
	void Update();
	
	enum GameMode
	{
		MENU, 
		MENUSLIDE,
		GAME,
		GAMEOVERSLIDE,
		GAMEOVER
	};

	GameMode mode = MENU;

private:

	MusicMixer *mixer;

	King::Engine mEngine;
	GemGrid* grid;
	gemGrid8x8& gemGrid;
	Renderer* renderer;
	
	int score;

	float referenceClock;
	bool gemLocked;
	bool swipePerformed;
	bool mouseDownAlreadyHandled;

	int selectedGemX;
	int selectedGemY;
	int switchGemX;
	int switchGemY;

	float slidePositionX;
	float slidePositionY;
	float slideIncrementer;

	Gem::direction gemDirection;

	Gem* clickedGem;
	Gem* switchingGem;
	Gem* previousSwitchingGem;

	void handleUserInteraction();
	void Game::handleGemClick();
	void Game::handleGemSwipe();

	Gem::direction getMouseDirection(std::pair<float, float> startMousePosition) const;
	bool Game::isClickInsideGameArea() const;
	std::pair<float, Gem::direction> getGemOffset(Gem &gem) const;
	void Game::resetClickedGemCoordinates();
	bool Game::isOffsetBigEnoughForSwap() const;
	void Game::resetSwappingGemPositions();
	bool Game::isOffsetBigEnoughForSwipe() const;
	bool Game::isGemNeighbour (int gemX, int gemY) const;
	void Game::selectGem(int gemX, int gemY);
	void Game::handleMenuSlideDown();
	void Game::handleGameOverSlideUp();
	void Game::startNewGame();


	
	const int gameDuration = 10;
};
