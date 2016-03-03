#include "Game.h"
#include "Renderer.h"

Game::Game(GemGrid& passedGrid)
	: mEngine("./assets")
	, mRotation(0.0f)
	, mYellowDiamondX(100.0f)
	, mYellowDiamondY(100.0f)
	, grid(&passedGrid)
	, gridArray(grid->getGemGrid())
	, renderer(new Renderer(mEngine, *grid))
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

	int x = 0;
	int y = 0;

	if (IsGemClicked())
	{
		switch (gemDirection)
		{
		case Renderer::HORIZONTAL_RIGHT:
			
			if (selectedGemX < 7)
			{
				// Ako smo otišli predaleko desno
				if (abs(mEngine.GetMouseX() - moseMoveStartX) >= grid->gridOffset - 10)
				{
					renderer->RenderGemGrid(selectedGemX, selectedGemY, grid->gridOffset, 0, Renderer::STATIONARY);
					switchGemX = 1;
					switchGemY = 0;
				}
				else
				{
					renderer->RenderGemGrid(selectedGemX, selectedGemY, mEngine.GetMouseX() - moseMoveStartX, 0, Renderer::HORIZONTAL_RIGHT);
				}
			}
			else
			{
				renderer->RenderGemGrid();
			}
			break;
		case Renderer::HORIZONTAL_LEFT:
			// Otisli predaleko lijevo
			if (selectedGemX > 0)
			{
				if (abs(mEngine.GetMouseX() - moseMoveStartX) >= grid->gridOffset - 10)
				{
					renderer->RenderGemGrid(selectedGemX, selectedGemY, -grid->gridOffset, 0, Renderer::HORIZONTAL_LEFT);
					switchGemX = -1;
					switchGemY = 0;
				}
				
				else
				{
					renderer->RenderGemGrid(selectedGemX, selectedGemY, mEngine.GetMouseX() - moseMoveStartX, 0, Renderer::HORIZONTAL_LEFT);
				}
			}
			else
			{
				renderer->RenderGemGrid();
			}
			break;
		case Renderer::VERTICAL_UP:
			if (selectedGemY < 7)
			{
				if (abs(mEngine.GetMouseY() - moseMoveStartY) >= grid->gridOffset - 10)
				{
					renderer->RenderGemGrid(selectedGemX, selectedGemY, 0, grid->gridOffset, Renderer::STATIONARY);
					switchGemX = 0;
					switchGemY = 1;
				}
				else
				{
					renderer->RenderGemGrid(selectedGemX, selectedGemY, 0, mEngine.GetMouseY() - moseMoveStartY, Renderer::VERTICAL_UP);
				}
			}
			else
			{
				renderer->RenderGemGrid();
			}
			break;
		case Renderer::VERTICAL_DOWN:
			if(selectedGemY > 0) 
			{
				if (abs(mEngine.GetMouseY() - moseMoveStartY) >= grid->gridOffset - 10)
				{
					renderer->RenderGemGrid(selectedGemX, selectedGemY, 0, -grid->gridOffset, Renderer::STATIONARY);
					switchGemX = 0;
					switchGemY = -1;
				}
				else
				{
					renderer->RenderGemGrid(selectedGemX, selectedGemY, 0, mEngine.GetMouseY() - moseMoveStartY, Renderer::VERTICAL_DOWN);
				}
			}
			else
			{
				renderer->RenderGemGrid();
			}
			break;
		case Renderer::OUT_OF_BOUNDS:
			renderer->RenderGemGrid();
			break;
		}
	
	}
	else
	{
		if (switchGemX != 0 || switchGemY != 0)
		{
			int gemToSwitch = gridArray[selectedGemX][selectedGemY];
			gridArray[selectedGemX][selectedGemY] = gridArray[selectedGemX + switchGemX][selectedGemY + switchGemY];
			gridArray[selectedGemX + switchGemX][selectedGemY + switchGemY] = gemToSwitch;
			switchGemX = 0;
			switchGemY = 0;
			objectLocked = false;

		}
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
				// Ako ti je mis otisa horizontalno
				if (abs(mEngine.GetMouseX() - moseMoveStartX) >= abs(mEngine.GetMouseY() - moseMoveStartY))
				{
					if (mEngine.GetMouseX() > moseMoveStartX)
					{
							gemDirection = Renderer::HORIZONTAL_RIGHT;
					}
					else
					{
							gemDirection = Renderer::HORIZONTAL_LEFT;
					}
				}
				// Ako ti je mis otisa vertikalno
				else
				{
					if (mEngine.GetMouseY() > moseMoveStartY)
					{
							gemDirection = Renderer::VERTICAL_UP;
					}
					else
					{
							gemDirection = Renderer::VERTICAL_DOWN;
					}
				}
			}
		}
		else
		{
			gemDirection = Renderer::OUT_OF_BOUNDS;
			objectLocked = false;
		}
	}
	else
	{
		objectLocked = false;
	}

	return objectLocked;
}