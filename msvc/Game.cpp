#include "Game.h"


Game::Game()
	: mEngine("./assets")
	, mRotation(0.0f)
	, score(0)
	, grid(new GemGrid())
	, gridArray(grid->GetGemGrid())
	, renderer(new Renderer(mEngine, *grid))
	, selectedGemX(-1)
	, selectedGemY(-1)
{};

Game::~Game()
{
	delete renderer;
	delete grid;
}

void Game::Start()
{
	mEngine.Start(*this);
}

void Game::Update()
{
	switch (mode)
	{
		case MENU:
		{
			renderer->RenderStartScreen();
				mEngine.Write("Click to start", 300.0f, 330.0f);

			if (mEngine.GetMouseButtonDown())
			{
				mode = MENUSLIDE;
			}
			break;
		}
		case MENUSLIDE:
		{
			renderer->RenderBackground();
			renderer->RenderStartScreen(slidePositionX, slidePositionY);
			slidePositionY += slideIncrementer;
			++slideIncrementer;

			if (slidePositionY > mEngine.GetHeight())
			{
				mode = GAME;
				// Prepare sliding parameters to game over sliding
				slidePositionY = mEngine.GetHeight();
				slideIncrementer = 0.0f;
			}

			break;
		}
		case GAME:
		{
			// If gem was clicked to switch, perform the animation of switching
			if (grid->gemMoving)
			{
				grid->AnimateGemSwitch();
			}

			renderer->RenderBackground();

			referenceClock += mEngine.GetLastFrameSeconds();

			renderer->RenderGemGrid();

			// If grid is not locked, perform handling of user interaction 
			if (!grid->IsGridLocked())
			{
				HandleGemInteraction();

				if (grid->checkPossibleMoves)
				{
					possibleMoves moves = grid->FindPossibleMoves();
					std::cout << "checking ... " << std::endl;

					if (moves.size() == 0)
					{

					}

				}

				if (grid->IsCascadePresent())
				{
					renderer->RenderToBeDestroyed(grid->GetGemsToDestroy());

					if (!renderer->MatchRendererInProgress)
					{
						grid->ActivateGravity();
						score += grid->DestroyGems();
					}
				}
			}
			// If grid is locked, perform gravity pull
			else
			{
				grid->GravityPull();
			}

			// Draw score
			mEngine.Write("Score", 50.0f, 25.0f);
			std::string s = std::to_string(score);
			char const *pchar = s.c_str();
			mEngine.Write(pchar, 50.0f, 50.0f);

			// Draw time
			s = std::to_string(gameDuration - static_cast<int>(referenceClock));
			pchar = s.c_str();
			mEngine.Write(pchar, 100.0f, 450.0f);

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
			renderer->RenderBackground();
			renderer->RenderStartScreen(slidePositionX, slidePositionY);
			slidePositionY -= slideIncrementer;
			++slideIncrementer;

			if (slidePositionY < 0)
			{
				mode = GAMEOVER;
				// Reset sliding parameters after sliding is over
				slidePositionY = 0.0f;
			}
			break;
		}
		case GAMEOVER:
		{
			renderer->RenderStartScreen();

			std::string s = std::to_string(score);
			char const *pchar = s.c_str();
			mEngine.Write("Score:", 330.0f, 330.0f);
			mEngine.Write(pchar, 410.0f, 330.0f);
			mEngine.Write("Click to retry", 300.0f, 360.0f);

			if (mEngine.GetMouseButtonDown())
			{
				score = 0;
				mode = MENUSLIDE;
				delete grid;
				grid = new GemGrid();
			}
			break;
		}
	}
}

bool Game::HandleGemInteraction()
{
	// If mouse is down
	if (mEngine.GetMouseButtonDown())
	{
		// If click was inside the gem grid
		if (isClickInsideGameArea())
		{
			handleGemClick();
		}
		// If click was outside of the grid
		//else
		//{
		//	resetClickedGemCoordinates();
		//}
		clickDown = true;
	}

	// If mouse is up
	else
	{
		clickDown = false;
		// If mouse click is up, swipe is over
		swipePerformed = false;

		// Now, perform switching if gem offset after swipe is big enough
		if (clickedGem != nullptr && switchingGem != nullptr)
		{
			if (abs(clickedGem->GetOffsetX()) > 15.f ||
				abs(clickedGem->GetOffsetY()) > 15.f)
			{

				grid->SwitchGems(selectedGemX, selectedGemY, switchGemX, switchGemY);
				resetClickedGemCoordinates();
			}
			else
			{
				// If no switch is performed, return the gems to their original positions
				gridArray[selectedGemX][selectedGemY]->SetOffset(0, 0);
				gridArray[switchGemX][switchGemY]->SetOffset(0, 0);
			}
		}
	}

	// If gem is locked, and swipe is not active, render the selection box
	if (gemLocked && !swipePerformed) renderer->RenderSelected(selectedGemX, selectedGemY);

	return false;
}

void Game::handleGemClick()
{
	int gemX = static_cast<int>((mEngine.GetMouseX() - grid->gridXStart) / grid->gridOffset);
	int gemY = static_cast<int>((mEngine.GetMouseY() - grid->gridYStart) / grid->gridOffset);

	// If the click was on the same gem
	if (gemX == selectedGemX && gemY == selectedGemY)
	{
		// Do not perform swipe until user tries to swap for more than 1.0f of distance
		if (swipePerformed ||
			getGemOffset(*gridArray[selectedGemX][selectedGemY]).first > 1.0f ||
			getGemOffset(*gridArray[selectedGemX][selectedGemY]).first < -1.0f)
		{
			handleGemSwipe();
		}
		// Element was deselected
		else if (clickDown == false)
		{
			std::cout << "Click" << std::endl;
			resetClickedGemCoordinates();
		}
	}

	// If the click was on different gem
	else if (gemX != selectedGemX || gemY != selectedGemY)
	{
		if (clickDown == false)
		{
			// If it is neighbour, switch gems
			if (((abs(selectedGemX - gemX) == 1 && abs(selectedGemY - gemY) == 0) ||
				(abs(selectedGemY - gemY) == 1 && abs(selectedGemX - gemX) == 0)) &&
				gemLocked)
			{

				grid->SwitchGems(selectedGemX, selectedGemY, gemX, gemY);
				resetClickedGemCoordinates();
			}
			// If it is not neighbour, select the clicked gem
			else
			{
				selectedGemX = gemX;
				selectedGemY = gemY;
				gemLocked = true;
				gridArray[selectedGemX][selectedGemY]->mouseMoveStartX = mEngine.GetMouseX();
				gridArray[selectedGemX][selectedGemY]->mouseMoveStartY = mEngine.GetMouseY();
			}
		}
		else
		{
			handleGemSwipe();
		}
	}
}

void Game::handleGemSwipe()
{
	if (gemLocked)
	{
		swipePerformed = true;
		clickedGem = gridArray[selectedGemX][selectedGemY];

		gemDirection = getMouseDirection(clickedGem->mouseMoveStartX, clickedGem->mouseMoveStartY);
		std::pair<float, Gem::direction> offsetDirection = getGemOffset(*clickedGem);

		switch (offsetDirection.second)
		{
		case Gem::HORIZONTAL_LEFT:
			switchGemX = selectedGemX - 1;
			switchGemY = selectedGemY;
			switchingGem = gridArray[selectedGemX - 1][selectedGemY];
			clickedGem->SetOffset(offsetDirection.first, 0);
			switchingGem->SetOffset(-offsetDirection.first, 0);
			break;
		case Gem::HORIZONTAL_RIGHT:
			switchGemX = selectedGemX + 1;
			switchGemY = selectedGemY;
			switchingGem = gridArray[selectedGemX + 1][selectedGemY];
			clickedGem->SetOffset(-offsetDirection.first, 0);
			switchingGem->SetOffset(offsetDirection.first, 0);
			break;
		case Gem::VERTICAL_UP:
			switchGemX = selectedGemX;
			switchGemY = selectedGemY - 1;
			switchingGem = gridArray[selectedGemX][selectedGemY - 1];
			clickedGem->SetOffset(0, offsetDirection.first);
			switchingGem->SetOffset(0, -offsetDirection.first);
			break;
		case Gem::VERTICAL_DOWN:
			switchGemX = selectedGemX;
			switchGemY = selectedGemY + 1;
			switchingGem = gridArray[selectedGemX][selectedGemY + 1];
			clickedGem->SetOffset(0, -offsetDirection.first);
			switchingGem->SetOffset(0, offsetDirection.first);
			break;
		default:
			clickedGem->ResetOffset();
		}

		if (switchingGem != previousSwitchingGem
			&& previousSwitchingGem != nullptr)
		{
			previousSwitchingGem->ResetOffset();
		}
		previousSwitchingGem = switchingGem;
	}
}

void Game::resetClickedGemCoordinates()
{
	switchGemX = -1;
	switchGemY = -1;
	selectedGemX = -1;
	selectedGemY = -1;

	gemLocked = false;
	clickedGem = nullptr;
	switchingGem = nullptr;
}

Gem::direction Game::getMouseDirection(float mouseStartPositionX, float mouseStartPositionY)
{
	// Ako ti je mis otisa horizontalno
	if (abs(mEngine.GetMouseX() - mouseStartPositionX) >= abs(mEngine.GetMouseY() - mouseStartPositionY))
	{
		if (mEngine.GetMouseX() > mouseStartPositionX)
		{
			return Gem::HORIZONTAL_RIGHT;
		}
		else
		{
			return Gem::HORIZONTAL_LEFT;
		}
	}
	// Ako ti je mis otisa vertikalno
	else
	{
		if (mEngine.GetMouseY() > mouseStartPositionY)
		{
			return Gem::VERTICAL_DOWN;
		}
		else
		{
			return Gem::VERTICAL_UP;
		}
	}
}

bool Game::isClickInsideGameArea()
{
	if (mEngine.GetMouseX() > grid->gridXStart
		&& mEngine.GetMouseX() < grid->gridXStart + (static_cast<float>(GRID_WIDTH) * grid->gridOffset)
		&& mEngine.GetMouseY() > grid->gridYStart
		&& mEngine.GetMouseY() < grid->gridYStart + (static_cast<float>(GRID_WIDTH) * grid->gridOffset))
		return true;

	return false;
}

std::pair<float, Gem::direction> Game::getGemOffset(Gem &gem)
{
	int mouseStartX = gem.mouseMoveStartX;
	int mouseStartY = gem.mouseMoveStartY;

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