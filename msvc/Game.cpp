#include "Game.h"


Game::Game()
	: mEngine("./assets")
	, mRotation(0.0f)
	, mYellowDiamondX(100.0f)
	, mYellowDiamondY(100.0f)
	, grid(new GemGrid())
	, gridArray(grid->getGemGrid())
	, renderer(new Renderer(mEngine, *grid))
	, selectedGemX(0)
	, selectedGemY(0)
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
	renderer->RenderBackground();

	mRotation += mEngine.GetLastFrameSeconds();

	if (IsAnyGemClicked())
	{
		renderer->RenderGemGrid();
	}
	else
	{
		/*if (switchGemX != 0 || switchGemY != 0)
		{
			Gem* gemToSwitch = gridArray[selectedGemX][selectedGemY];
			gridArray[selectedGemX][selectedGemY] = gridArray[selectedGemX + switchGemX][selectedGemY + switchGemY];
			gridArray[selectedGemX + switchGemX][selectedGemY + switchGemY] = gemToSwitch;
			switchGemX = 0;
			switchGemY = 0;
			gemLocked = false;

		}*/
		renderer->RenderGemGrid();
	}

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

}

bool Game::IsAnyGemClicked()
{
	if (mEngine.GetMouseButtonDown())
	{
		// If the click was inside the grid
		if (IsClickInsideGameArea())
		{
			if (!gemLocked)
			{
				selectedGemX = static_cast<int>((mEngine.GetMouseX() - grid->gridXStart) / grid->gridOffset);
				selectedGemY = static_cast<int>((mEngine.GetMouseY() - grid->gridYStart) / grid->gridOffset);
			}

			clickedGem = gridArray[selectedGemX][selectedGemY];

			if (!clickedGem->Selected)
			{
				clickedGem->Selected = true;
				clickedGem->mouseMoveStartX = mEngine.GetMouseX();
				clickedGem->mouseMoveStartY = mEngine.GetMouseY();
				gemLocked = true;
			}
			else
			{
				gemDirection = GetMouseDirection(clickedGem->mouseMoveStartX, clickedGem->mouseMoveStartY);
				std::pair<float, Gem::direction> offsetDirection = GetGemOffset(clickedGem->mouseMoveStartX, clickedGem->mouseMoveStartY);
				
				//Setiraj offset gema
				switch (offsetDirection.second)
				{
				case Gem::HORIZONTAL_LEFT:
					if (swithingGem != nullptr)
					{
						swithingGem->ResetOffset();
					}
					swithingGem = gridArray[selectedGemX - 1][selectedGemY];
					clickedGem->SetOffsetX(offsetDirection.first);
					swithingGem->SetOffsetX(-offsetDirection.first);
					clickedGem->SetOffsetY(0);
					swithingGem->SetOffsetY(0);

					std::cout << clickedGem->GetGemColor() << ":" << swithingGem->GetGemColor() << std::endl;
					break;
				case Gem::HORIZONTAL_RIGHT:
					if (swithingGem != nullptr)
					{
						swithingGem->ResetOffset();
					}
					swithingGem = gridArray[selectedGemX + 1][selectedGemY];
					clickedGem->SetOffsetX(-offsetDirection.first);
					swithingGem->SetOffsetX(offsetDirection.first);
					clickedGem->SetOffsetY(0);
					swithingGem->SetOffsetY(0);
					std::cout << clickedGem->GetGemColor() << ":" << swithingGem->GetGemColor() << std::endl;
					break;
				case Gem::VERTICAL_UP:
					if (swithingGem != nullptr)
					{
						swithingGem->ResetOffset();
					}
					swithingGem = gridArray[selectedGemX][selectedGemY - 1];
					clickedGem->SetOffsetY(offsetDirection.first);
					swithingGem->SetOffsetY(-offsetDirection.first);
					clickedGem->SetOffsetX(0);
					swithingGem->SetOffsetX(0);
					std::cout << clickedGem->GetGemColor() << ":" << swithingGem->GetGemColor() << std::endl;
					break;
				case Gem::VERTICAL_DOWN:
					if (swithingGem != nullptr)
					{
						swithingGem->ResetOffset();
					}
					swithingGem = gridArray[selectedGemX][selectedGemY + 1];
					clickedGem->SetOffsetY(-offsetDirection.first);
					swithingGem->SetOffsetY(offsetDirection.first);
					clickedGem->SetOffsetX(0);
					swithingGem->SetOffsetX(0);
					std::cout << clickedGem->GetGemColor() << ":"<< swithingGem->GetGemColor() << std::endl;
					break;
				default:
					clickedGem->ResetOffset();
				}
			}
		}
		else
		{
			gemDirection = Gem::OUT_OF_BOUNDS;
		}
	}
	else
	{
		if (clickedGem != nullptr)
		{
			clickedGem->Selected = false;
			clickedGem->ResetOffset();
		}

		if (swithingGem != nullptr)
		{
			swithingGem->ResetOffset();
		}

		switch (gemDirection)
		{
		case Gem::HORIZONTAL_LEFT:

			break;
		case Gem::HORIZONTAL_RIGHT:

			break;
		case Gem::VERTICAL_UP:

			break;
		case Gem::VERTICAL_DOWN:

			break;
		default:
			break;
		}

		gemLocked = false;
		clickedGem = nullptr;
		swithingGem = nullptr;
	}

	return gridArray[selectedGemX][selectedGemY]->Selected;
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


std::pair<float, Gem::direction> Game::GetGemOffset(int mouseStartX, int mouseStartY)
{
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