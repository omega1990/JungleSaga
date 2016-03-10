#include "Game.h"


Game::Game()
	: mEngine("./assets")
	, grid(new GemGrid())
	, gemGrid(grid->GetGemGrid())
	, renderer(new Renderer(mEngine, *grid))
	, score(0)
	, selectedGemX(DEFAULT_COORDINATE)
	, selectedGemY(DEFAULT_COORDINATE)
	, switchGemX(0)
	, switchGemY(0)
	, referenceClock(0.0f)
	, gemLocked(false)
	, swipePerformed(false)
	, mouseDownAlreadyHandled(false)
	, clickedGem(nullptr)
	, switchingGem(nullptr)
	, previousSwitchingGem(nullptr)
	, slidePositionX(0.0f)
	, slidePositionY(0.0f)
	, slideIncrementer(0.0f)
{};

Game::~Game()
{
	delete renderer;
	delete grid;
	Mix_FreeMusic(music);
	Mix_FreeChunk(gemHit);
}

void Game::Start()
{
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("./assets/music.mp3");
	if (!music)
	{
		std::cout << "Error while loading music " << Mix_GetError() << std::endl;
	}
	Mix_PlayMusic(music, -1);

	gemHit = Mix_LoadWAV("./assets/gemPing.wav");
	if (!gemHit)
	{
		std::cout << "Error while loading effect " << Mix_GetError() << std::endl;
	}

	mEngine.Start(*this);
	
	
}

void Game::Update()
{
	switch (mode)
	{
		case MENU:
		{
			renderer->RenderMenu();
			handleUserInteraction();
			break;
		}
		case MENUSLIDE:
		{
			renderer->RenderMenuSlide(slidePositionX, slidePositionY);
			handleMenuSlideDown();
			break;
		}
		case GAME:
		{
			// If gem was clicked to switch, perform the animation of switching
			if (grid->IsGemMoving())
			{
				grid->AnimateGemSwitch();
			}

			renderer->RenderBackground();

			referenceClock += mEngine.GetLastFrameSeconds();

			renderer->RenderGemGrid();

			// If grid is not locked, perform handling of user interaction 
			if (!grid->IsGridLocked())
			{
				handleUserInteraction();

				if (grid->possibleCheckPending())
				{
					possibleMoves moves = grid->FindPossibleMoves();
					std::cout << "checking ... " << std::endl;

					if (moves.size() == 0)
					{
						grid->Reshuffle();
					}
				}

				if (grid->IsCascadePresent())
				{
					renderer->RenderToBeDestroyed(grid->GetGemsToDestroy());

					if (!renderer->IsMatchRenderingInProgress())
					{
						grid->ActivateGravity();
						score += grid->DestroyGems();
						Mix_PlayChannel(1, gemHit, 0);
					}
				}
			}
			// If grid is locked, perform gravity pull
			else
			{
				grid->GravityPull();
			}

			// Draw score
			std::string s = std::to_string(score);
			char const *cscore = s.c_str();
			renderer->RenderScore(cscore);

			// Draw time
			s = std::to_string(gameDuration - static_cast<int>(referenceClock));
			char const *ctime = s.c_str();
			renderer->RenderTime(ctime);

			if (static_cast<int>(referenceClock) == gameDuration)
			{
				mode = GAMEOVERSLIDE;
				referenceClock = 0.0f;
			}

			// Render the top of the grid so it is above gems falling from the top
			renderer->RenderTop();
			break;
		}
		case GAMEOVERSLIDE:
		{
			renderer->RenderGameOverSlide(slidePositionX, slidePositionY);
			handleGameOverSlideUp();			
			break;
		}
		case GAMEOVER:
		{
			std::string s = std::to_string(score);
			char const *pchar = s.c_str();
			renderer->RenderGameOver(pchar);
			handleUserInteraction();
			break;
		}
	}
}

/// <summary> handles user interaction with the board </summary>
void Game::handleUserInteraction()
{
	// If mouse is down
	if (mEngine.GetMouseButtonDown())
	{
		// Handle user interaction to switch between game modes
		switch (mode)
		{
		case MENU:
			mode = MENUSLIDE;
			return;
		case GAMEOVER:
			if(!mouseDownAlreadyHandled)
				startNewGame();
			return;
		}

		// If click was inside the gem grid handle it
		if (isClickInsideGameArea())
		{
			handleGemClick();
		}
		mouseDownAlreadyHandled = true;
	}

	// If mouse is up
	else
	{
		// If mouse click is up, next we handle is the first click down
		mouseDownAlreadyHandled = false;

		// If mouse click is up, swipe is over
		swipePerformed = false;

		// Now, perform switching if gem offset after swipe is big enough
		if (clickedGem != nullptr && switchingGem != nullptr)
		{
			if (isOffsetBigEnoughForSwap())
			{
				grid->SwitchGems(selectedGemX, selectedGemY, switchGemX, switchGemY);
				resetClickedGemCoordinates();
			}
			else
			{
				// If no switch is performed, return the gems to their original positions
				resetSwappingGemPositions();
			}
		}
	}

	// If gem is locked, and swipe is not active, render the selection box
	if (gemLocked && !swipePerformed && mode != GAMEOVER) renderer->RenderSelected(selectedGemX, selectedGemY);
}

/// <summary> Handles clicking on the board </summary>
void Game::handleGemClick()
{
	int gemX = static_cast<int>((mEngine.GetMouseX() - grid->gridXStart) / grid->gridOffset);
	int gemY = static_cast<int>((mEngine.GetMouseY() - grid->gridYStart) / grid->gridOffset);

	// If the click was on the same gem
	if (gemX == selectedGemX && gemY == selectedGemY)
	{
		// Do not perform swipe until user tries to swap for more than 1.0f of distance
		if (swipePerformed || isOffsetBigEnoughForSwipe())
		{
			handleGemSwipe();
		}
		// Element was deselected
		else if (mouseDownAlreadyHandled == false)
		{
			resetClickedGemCoordinates();
		}
	}

	// If the click was on different gem
	else if (gemX != selectedGemX || gemY != selectedGemY)
	{
		if (mouseDownAlreadyHandled == false)
		{
			// If it is neighbour, switch gems
			if (isGemNeighbour(gemX, gemY) && gemLocked)
			{
				grid->SwitchGems(selectedGemX, selectedGemY, gemX, gemY);
				resetClickedGemCoordinates();
			}
			// If it is not neighbour, select the clicked gem
			else
			{
				selectGem(gemX, gemY);
			}
		}
		// If mouse is down for more than one frame, handle it as swipe
		else
		{
			handleGemSwipe();
		}
	}
}

/// <summary> Handles swiping on the board </summary>
void Game::handleGemSwipe()
{
	if (gemLocked)
	{
		swipePerformed = true;
		clickedGem = gemGrid[selectedGemX][selectedGemY];

		gemDirection = getMouseDirection(clickedGem->getMouseMoveStart());
		std::pair<float, Gem::direction> offsetDirection = getGemOffset(*clickedGem);

		switch (offsetDirection.second)
		{
		case Gem::HORIZONTAL_LEFT:
			switchGemX = selectedGemX - 1;
			switchGemY = selectedGemY;
			switchingGem = gemGrid[selectedGemX - 1][selectedGemY];
			clickedGem->SetOffset(offsetDirection.first, 0);
			switchingGem->SetOffset(-offsetDirection.first, 0);
			break;
		case Gem::HORIZONTAL_RIGHT:
			switchGemX = selectedGemX + 1;
			switchGemY = selectedGemY;
			switchingGem = gemGrid[selectedGemX + 1][selectedGemY];
			clickedGem->SetOffset(-offsetDirection.first, 0);
			switchingGem->SetOffset(offsetDirection.first, 0);
			break;
		case Gem::VERTICAL_UP:
			switchGemX = selectedGemX;
			switchGemY = selectedGemY - 1;
			switchingGem = gemGrid[selectedGemX][selectedGemY - 1];
			clickedGem->SetOffset(0, offsetDirection.first);
			switchingGem->SetOffset(0, -offsetDirection.first);
			break;
		case Gem::VERTICAL_DOWN:
			switchGemX = selectedGemX;
			switchGemY = selectedGemY + 1;
			switchingGem = gemGrid[selectedGemX][selectedGemY + 1];
			clickedGem->SetOffset(0, -offsetDirection.first);
			switchingGem->SetOffset(0, offsetDirection.first);
			break;
		default:
			clickedGem->ResetOffset();
			break;
		}

		if (switchingGem != previousSwitchingGem
			&& previousSwitchingGem != nullptr)
		{
			previousSwitchingGem->ResetOffset();
		}
		previousSwitchingGem = switchingGem;
	}
}

/// <summary> Resets coordinates stored for currently selected and switching gem to defaults </summary>
void Game::resetClickedGemCoordinates()
{
	switchGemX = DEFAULT_COORDINATE;
	switchGemY = DEFAULT_COORDINATE;
	selectedGemX = DEFAULT_COORDINATE;
	selectedGemY = DEFAULT_COORDINATE;

	gemLocked = false;
	clickedGem = nullptr;
	switchingGem = nullptr;
}

/// <summary> Gets the direction in which mouse went during the mouse </summary>
Gem::direction Game::getMouseDirection(std::pair<float, float> startMousePosition) const 
{
	// If mouse went somewhere horizontally
	if (abs(mEngine.GetMouseX() - startMousePosition.first) >= abs(mEngine.GetMouseY() - startMousePosition.second))
	{
		if (mEngine.GetMouseX() > startMousePosition.first)
		{
			return Gem::HORIZONTAL_RIGHT;
		}
		else
		{
			return Gem::HORIZONTAL_LEFT;
		}
	}
	// If mouse went somewhere vertically
	else
	{
		if (mEngine.GetMouseY() > startMousePosition.second)
		{
			return Gem::VERTICAL_DOWN;
		}
		else
		{
			return Gem::VERTICAL_UP;
		}
	}
}

/// <summary> Checks if click was inside game area </summary>
bool Game::isClickInsideGameArea() const
{
	if (mEngine.GetMouseX() > grid->gridXStart
		&& mEngine.GetMouseX() < grid->gridXStart + (static_cast<float>(GRID_WIDTH) * grid->gridOffset)
		&& mEngine.GetMouseY() > grid->gridYStart
		&& mEngine.GetMouseY() < grid->gridYStart + (static_cast<float>(GRID_WIDTH) * grid->gridOffset))
		return true;

	return false;
}

/// <summary> Gets current gem offset from it's natural position </summary>
std::pair<float, Gem::direction> Game::getGemOffset(Gem &gem) const
{
	int mouseStartX = gem.getMouseMoveStart().first;
	int mouseStartY = gem.getMouseMoveStart().second;

	switch (gemDirection)
	{
	case Gem::HORIZONTAL_LEFT:
		if (selectedGemX == 0) return std::make_pair(0, Gem::STATIONARY);
		return std::make_pair(mEngine.GetMouseX() - mouseStartX, gemDirection);
	case Gem::HORIZONTAL_RIGHT:
		if (selectedGemX == GRID_WIDTH - 1) return std::make_pair(0, Gem::STATIONARY);
		return  std::make_pair(mouseStartX - mEngine.GetMouseX(), gemDirection);
	case Gem::VERTICAL_UP:
		if (selectedGemY == 0) return std::make_pair(0, Gem::STATIONARY);
		return std::make_pair(mEngine.GetMouseY() - mouseStartY, gemDirection);
	case Gem::VERTICAL_DOWN:
		if (selectedGemY == GRID_HEIGHT - 1) return std::make_pair(0, Gem::STATIONARY);
		return std::make_pair(mouseStartY - mEngine.GetMouseY(), gemDirection);
	default:
		return std::make_pair(0, Gem::STATIONARY);
	}
}

/// <summary> Check if gem was moved enough to swap gems </summary>
bool Game::isOffsetBigEnoughForSwap() const
{
	if (abs(clickedGem->GetOffsetX()) > OFFSET_FOR_SWAP ||
		abs(clickedGem->GetOffsetY()) > OFFSET_FOR_SWAP)
		return true;
	else
		return false;
}

/// <summary> Returns selected and switching gem to their original positions </summary>
void Game::resetSwappingGemPositions()
{
	gemGrid[selectedGemX][selectedGemY]->SetOffset(0, 0);
	gemGrid[switchGemX][switchGemY]->SetOffset(0, 0);
}

/// <summary> Check if gem was moved enough start the swipe movement </summary>
bool Game::isOffsetBigEnoughForSwipe() const
{
	if (getGemOffset(*gemGrid[selectedGemX][selectedGemY]).first > OFFSET_FOR_SWIPE ||
		getGemOffset(*gemGrid[selectedGemX][selectedGemY]).first < -OFFSET_FOR_SWIPE)
		return true;
	else
		return false;
}

/// <summary> Check if gem with x and y coordinates is neighbour of currently selected gem </summary>
bool Game::isGemNeighbour(int gemX, int gemY) const
{
	if ((abs(selectedGemX - gemX) == 1 && abs(selectedGemY - gemY) == 0) ||
		(abs(selectedGemY - gemY) == 1 && abs(selectedGemX - gemX) == 0))
		return true;
	else
		return false;
}

/// <summary> Set the gem with specified coordinates as selected </summary>
void Game::selectGem(int gemX, int gemY)
{
	selectedGemX = gemX;
	selectedGemY = gemY;
	gemLocked = true;
	
	gemGrid[selectedGemX][selectedGemY]->SetMouseMoveStart(mEngine.GetMouseX(), mEngine.GetMouseY());
}

/// <summary> Handle sliding of the start menu </summary>
void Game::handleMenuSlideDown()
{
	slidePositionY += slideIncrementer++;
	if (slidePositionY > mEngine.GetHeight())
	{
		mode = GAME;
		// Prepare sliding parameters to game over sliding
		slidePositionY = mEngine.GetHeight();
		slideIncrementer = 0.0f;
	}
}

/// <summary> Handle sliding of the game over menu </summary>
void Game::handleGameOverSlideUp()
{
	slidePositionY -= slideIncrementer;
	++slideIncrementer;

	if (slidePositionY < 0)
	{
		mode = GAMEOVER;
		// Reset sliding parameters after sliding is over
		slidePositionY = 0.0f;
	}
}

/// <summary> Resets all fields to default values and re-initates gem grid </summary>
void Game::startNewGame()
{
	score = 0;
	mode = MENUSLIDE;
	
	score = 0;
	selectedGemX = DEFAULT_COORDINATE;
	selectedGemY = DEFAULT_COORDINATE;
	switchGemX = 0;
	switchGemY = 0;
	referenceClock = 0.0f;
	gemLocked = false;
	swipePerformed = false;
	mouseDownAlreadyHandled = false;
	clickedGem = nullptr;
	switchingGem = nullptr;
	previousSwitchingGem = nullptr;
	slidePositionX = 0.0f;
	slidePositionY = 0.0f;
	slideIncrementer = 0.0f; 

	grid->EmptyGrid();
	grid->InitializeGrid();
}