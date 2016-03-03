#include "Game.h"
#include "Renderer.h"

Game::Game(GemGrid& passedGrid)
	: mEngine("./assets")
	, mRotation(0.0f)
	, mYellowDiamondX(100.0f)
	, mYellowDiamondY(100.0f)
	, grid(&passedGrid)
	, gridArray(grid->getGemGrid())
	, renderer(new Renderer(mEngine,*grid))
{};

Game::~Game()
{
	delete renderer;
}

void Game::Start()
{
	mEngine.Start(*this);
}

void Game::Update()
{
	renderer->RenderBackground();

	mRotation += mEngine.GetLastFrameSeconds();
	
	if (IsGemClicked())
	{
		switch (gemDirection)
		{
		case HORIZONTAL:
			renderer->RenderGemGrid(selectedGemX, selectedGemY, mEngine.GetMouseX() - moseMoveStartX, 0);
			break;
		case VERTICAL:
			renderer->RenderGemGrid(selectedGemX, selectedGemY, 0, mEngine.GetMouseY() - moseMoveStartY);
			break;
		case STATIONARY: 
			renderer->RenderGemGrid();
			break;
		}	
	}
	else
	{
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

bool Game::IsGemClicked()
{
	if (mEngine.GetMouseButtonDown())
	{
		// If the click was inside the grid
		if (mEngine.GetMouseX() > grid->gridXStart && mEngine.GetMouseX() < grid->gridXStart + (8.0f * grid->gridOffset)
			&& mEngine.GetMouseY() > grid->gridYStart && mEngine.GetMouseY() < grid->gridYStart + (8.0f * grid->gridOffset))
		{
			if (!objectLocked)
			{
				selectedGemX = static_cast<int>((mEngine.GetMouseX() - grid->gridXStart) / grid->gridOffset);
				selectedGemY = static_cast<int>((mEngine.GetMouseY() - grid->gridYStart) / grid->gridOffset);
				moseMoveStartX = mEngine.GetMouseX();
				moseMoveStartY = mEngine.GetMouseY();				
				objectLocked = true;
			}
			else
			{
				if (abs(mEngine.GetMouseX() - moseMoveStartX) >= abs(mEngine.GetMouseY() - moseMoveStartY))
				{
					std::cout << "horizontal" << std::endl;
					gemDirection = HORIZONTAL;
				}
				else
				{
					std::cout << "vertical" << std::endl;
					gemDirection = VERTICAL;
				}
			}
		}
		else
		{
			gemDirection = STATIONARY;
		}
	}
	else
	{
		objectLocked = false;
	}

	return objectLocked;
}