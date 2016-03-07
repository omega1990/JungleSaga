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
	if (grid->gemMoving)
	{
		grid->AnimateGemSwitch();
	}

	renderer->RenderBackground();

	//mRotation += mEngine.GetLastFrameSeconds();

	referenceClock += mEngine.GetLastFrameSeconds();

	renderer->RenderGemGrid();

	if (!grid->IsGridLocked())
	{
		HandleGemInteraction();

		if (grid->IsCascadePresent())
		{
			renderer->RenderToBeDestroyed(grid->GetGemsToDestroy());

			//referenceClock += mEngine.GetLastFrameSeconds();
			//if ((referenceClock > 2.0f))
			//{
			score += grid->DestroyGems();
			//referenceClock = 0.0f;
		//}
		}
	}
	else
	{
		grid->GravityPull();
	}

	std::string s = std::to_string(score);
	char const *pchar = s.c_str();

	mEngine.Write(pchar, 50.0f, 50.0f);

	s = std::to_string(60 - static_cast<int>(referenceClock));
	pchar = s.c_str();
	mEngine.Write(pchar, 100.0f, 450.0f);

	//mEngine.Render(King::Engine::TEXTURE_BLUE, 650.0f, 450.0f);

	//mEngine.Write("Green", 650.0f, 140.0f);
	//mEngine.Write("Red", 100.0f, 490.0f);
	//mEngine.Write("Blue", 650.0f, 490.0f);

	//const char text[] = "This rotates at 5/PI Hz";
	//mRotation += mEngine.GetLastFrameSeconds();
	//mEngine.Write(text, mEngine.GetWidth() / 2.0f, mEngine.GetHeight() / 2.0f, mRotation * 20.5f);

	/*if (mEngine.GetMouseButtonDown()) {
	mYellowDiamondX = mEngine.GetMouseX();
	mYellowDiamondY = mEngine.GetMouseY();
	}
	mEngine.Render(King::Engine::TEXTURE_YELLOW, mYellowDiamondX, mYellowDiamondY);*/
	//mEngine.Write("Click to", mYellowDiamondX, mYellowDiamondY + 40.0f);
	//mEngine.Write("move me!", mYellowDiamondX, mYellowDiamondY + 70.0f);		

	//mEngine.Write("Jelena", mEngine.GetWidth() / 2.0f, mEngine.GetHeight() / 2.0f, mRotation * 20.5f);
	renderer->RenderTop();
}

bool Game::HandleGemInteraction()
{
	// If mouse is down
	if (mEngine.GetMouseButtonDown())
	{
		// If click was inside the gem grid
		if (IsClickInsideGameArea())
		{
			handleGemClick();
		}
		// If click was outside of the grid
		else
		{
			resetClickedGemCoordinates();
		}
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
			grid->TriggerGemMovingAnimation(selectedGemX, selectedGemY, switchGemX, switchGemY);
			resetClickedGemCoordinates();
		}
	}

	if (gemLocked && !swipePerformed) renderer->RenderSelected(selectedGemX, selectedGemY);
	
	//if (mEngine.GetMouseButtonDown())
	//{
	//	// If the click was inside the grid
	//	if (IsClickInsideGameArea())
	//	{
	//		if (!gemLocked)
	//		{
	//			selectedGemX = static_cast<int>((mEngine.GetMouseX() - grid->gridXStart) / grid->gridOffset);
	//			selectedGemY = static_cast<int>((mEngine.GetMouseY() - grid->gridYStart) / grid->gridOffset);
	//		}
	//		clickedGem = gridArray[selectedGemX][selectedGemY];
	//		if (!clickedGem->Selected)
	//		{
	//			clickedGem->Selected = true;
	//			clickedGem->mouseMoveStartX = mEngine.GetMouseX();
	//			clickedGem->mouseMoveStartY = mEngine.GetMouseY();
	//			gemLocked = true;
	//			firstGemLocked = true;
	//			// Set highlight texture if the gem is locked
	//			//clickedGem->SetGemColor(static_cast<King::Engine::Texture>(clickedGem->GetGemColor() + 1));
	//		}
	//		else
	//		{
	//			swipePerformed = true;
	//			gemDirection = GetMouseDirection(clickedGem->mouseMoveStartX, clickedGem->mouseMoveStartY);
	//			std::pair<float, Gem::direction> offsetDirection = GetGemOffset(clickedGem->mouseMoveStartX, clickedGem->mouseMoveStartY);
	//			if (swithingGem != nullptr)
	//			{
	//				swithingGem->ResetOffset();
	//			}
	//			if (clickedGem->mouseMoveStartX == mEngine.GetMouseX() && clickedGem->mouseMoveStartY == mEngine.GetMouseY())
	//			{
	//				swipePerformed = false;
	//				mEngine.Render(King::Engine::TEXTURE_SELECTED, grid->gridXStart + selectedGemX*grid->gridOffset - 4.0f, grid->gridYStart + selectedGemY*grid->gridOffset - 4.0f);
	//			}
	//			//Setiraj offset gema
	//			switch (offsetDirection.second)
	//			{
	//			case Gem::HORIZONTAL_LEFT:
	//				swithingGem = gridArray[selectedGemX - 1][selectedGemY];
	//				clickedGem->SetOffset(offsetDirection.first, 0);
	//				swithingGem->SetOffset(-offsetDirection.first, 0);
	//				break;
	//			case Gem::HORIZONTAL_RIGHT:
	//				swithingGem = gridArray[selectedGemX + 1][selectedGemY];
	//				clickedGem->SetOffset(-offsetDirection.first, 0);
	//				swithingGem->SetOffset(offsetDirection.first, 0);
	//				break;
	//			case Gem::VERTICAL_UP:
	//				swithingGem = gridArray[selectedGemX][selectedGemY - 1];
	//				clickedGem->SetOffset(0, offsetDirection.first);
	//				swithingGem->SetOffset(0, -offsetDirection.first);
	//				break;
	//			case Gem::VERTICAL_DOWN:
	//				swithingGem = gridArray[selectedGemX][selectedGemY + 1];
	//				clickedGem->SetOffset(0, -offsetDirection.first);
	//				swithingGem->SetOffset(0, offsetDirection.first);
	//				break;
	//			default:
	//				clickedGem->ResetOffset();
	//			}
	//		}
	//	}
	//	else
	//	{
	//		gemDirection = Gem::OUT_OF_BOUNDS;
	//	}
	//	clickReleased = false;
	//}
	//else
	//{
	//	if (!swipePerformed && firstGemLocked)
	//	{
	//		mEngine.Render(King::Engine::TEXTURE_SELECTED, grid->gridXStart + selectedGemX*grid->gridOffset - 4.0f, grid->gridYStart + selectedGemY*grid->gridOffset - 4.0f);
	//		clickReleased = true;
	//	}
	//	else if (clickedGem != nullptr && swithingGem != nullptr)
	//	{
	//		//clickedGem->SetGemColor(static_cast<King::Engine::Texture>(clickedGem->GetGemColor() - 1));
	//		clickedGem->Selected = false;
	//		bool performSwitch = false;
	//		if (clickedGem->GetOffsetX() > 10 ||
	//			clickedGem->GetOffsetY() > 10 ||
	//			clickedGem->GetOffsetX() < -10 ||
	//			clickedGem->GetOffsetY() < -10)
	//		{
	//			performSwitch = true;
	//		}
	//		clickedGem->ResetOffset();
	//		swithingGem->ResetOffset();
	//		if (performSwitch)
	//		{
	//			Gem* temp = new Gem(static_cast<King::Engine::Texture>(1));
	//			// Perform gem switching
	//			*temp = *clickedGem;
	//			*clickedGem = *swithingGem;
	//			*swithingGem = *temp;
	//			delete temp;
	//		}
	//		gemLocked = false;
	//		clickedGem = nullptr;
	//		swithingGem = nullptr;
	//	}
	//}

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
			GetGemOffset(*gridArray[selectedGemX][selectedGemY]).first > 1.0f ||
			GetGemOffset(*gridArray[selectedGemX][selectedGemY]).first < -1.0f)
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
				Gem* switchingGem;

				grid->TriggerGemMovingAnimation(selectedGemX, selectedGemY, gemX, gemY);
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

		gemDirection = GetMouseDirection(clickedGem->mouseMoveStartX, clickedGem->mouseMoveStartY);
		std::pair<float, Gem::direction> offsetDirection = GetGemOffset(*clickedGem);

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

Gem::direction Game::GetMouseDirection(float mouseStartPositionX, float mouseStartPositionY)
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

bool Game::IsClickInsideGameArea()
{
	if (mEngine.GetMouseX() > grid->gridXStart
		&& mEngine.GetMouseX() < grid->gridXStart + (static_cast<float>(GRID_WIDTH) * grid->gridOffset)
		&& mEngine.GetMouseY() > grid->gridYStart
		&& mEngine.GetMouseY() < grid->gridYStart + (static_cast<float>(GRID_WIDTH) * grid->gridOffset))
		return true;

	return false;
}

std::pair<float, Gem::direction> Game::GetGemOffset(Gem &gem)
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