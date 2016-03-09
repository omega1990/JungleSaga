#include "Renderer.h"

Renderer::Renderer(King::Engine& passedEngine, GemGrid& passedGrid)
	: mEngine(&passedEngine)
	, grid(&passedGrid)
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderStartScreen(float x, float y)
{
	mEngine->Render(King::Engine::TEXTURE_START, x, y);
}


void Renderer::RenderBackground()
{
	mEngine->Render(King::Engine::TEXTURE_BACKGROUND, 0, 0);
}

void Renderer::RenderTop()
{
	mEngine->Render(King::Engine::TEXTURE_TOP, 214, 0);
}

void Renderer::RenderGemGrid()
{
	gemGrid8x8& gridArray = grid->GetGemGrid();
	bool isSelected = false;
	int x;
	int y;

	// Draw complete gem grid
	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		for (int j = 0; j < GRID_HEIGHT; ++j)
		{
			if (gridArray[i][j]->Selected)
			{
				isSelected = true;
				x = i;
				y = j;
			}
			else
			{
				float offset;
				if (j <= grid->GetColumnOffsets().at(i).first && grid->GetColumnOffset(i) < 0)
				{
					offset = grid->GetColumnOffset(i);
				}
				else
				{
					offset = 0;
				}

				mEngine->Render(static_cast<King::Engine::Texture>(gridArray[i][j]->GetGemColor())
					, grid->gridXStart + i * grid->gridOffset + gridArray[i][j]->GetOffsetX()
					, grid->gridYStart + j * grid->gridOffset + gridArray[i][j]->GetOffsetY() + offset);
			}
		}
	}

	if (isSelected)
	{
		mEngine->Render(static_cast<King::Engine::Texture>(gridArray[x][y]->GetGemColor())
			, grid->gridXStart + x * grid->gridOffset + gridArray[x][y]->GetOffsetX()
			, grid->gridYStart + y * grid->gridOffset + gridArray[x][y]->GetOffsetY());
	}


}

void Renderer::RenderToBeDestroyed(std::vector<std::pair<int, int>> gemsToBeDestroyed)
{
	MatchRendererInProgress = true;

	// If more than 0.02 seconds passed
	if (clock > 0.02f)
	{
		clock = 0.0f;
		MatchRendererInProgress = false;
	}

	clock += mEngine->GetLastFrameSeconds();

	gemGrid8x8& gridArray = grid->GetGemGrid();

	for (auto gemCoordinade : gemsToBeDestroyed)
	{
		mEngine->Render(static_cast<King::Engine::Texture>(King::Engine::TEXTURE_TBD)
			, grid->gridXStart + gemCoordinade.first * grid->gridOffset + gridArray[gemCoordinade.first][gemCoordinade.second]->GetOffsetX()
			, grid->gridYStart + gemCoordinade.second * grid->gridOffset + gridArray[gemCoordinade.first][gemCoordinade.second]->GetOffsetY());
	}

	
}

void Renderer::RenderSelected(int selectedGemX, int selectedGemY)
{
	if (selectedGemX != -1 && selectedGemY != -1)
		mEngine->Render(King::Engine::TEXTURE_SELECTED, grid->gridXStart + selectedGemX*grid->gridOffset - 4.0f, grid->gridYStart + selectedGemY*grid->gridOffset - 4.0f);
}