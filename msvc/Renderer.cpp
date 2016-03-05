#include "Renderer.h"

Renderer::Renderer(King::Engine& passedEngine, GemGrid& passedGrid)
	: mEngine(&passedEngine)
	, grid(&passedGrid)
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderBackground()
{
	mEngine->Render(King::Engine::TEXTURE_BACKGROUND, 0, 0);
}

void Renderer::RenderGemGrid()
{
	gemGrid8x8& gridArray = grid->getGemGrid();
	bool isSelected = false;
	int x = -1;
	int y = -1;

	// Draw complete gem grid
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (gridArray[i][j]->GetOffsetX() > 0 || gridArray[i][j]->GetOffsetY() > 0)
			{
				isSelected = true;
				x = i;
				y = j;
			}
			else
			{
				mEngine->Render(static_cast<King::Engine::Texture>(gridArray[i][j]->GetGemColor())
					, grid->gridXStart + i * grid->gridOffset + gridArray[i][j]->GetOffsetX()
					, grid->gridYStart + j * grid->gridOffset + gridArray[i][j]->GetOffsetY());
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

void Renderer::RenderToBeDestroyed(std::vector<std::pair<int,int>> gemsToBeDestroyed)
{
	gemGrid8x8& gridArray = grid->getGemGrid();
	bool isSelected = false;
	int x = -1;
	int y = -1;

	for (auto gemCoordinade : gemsToBeDestroyed)
	{
		mEngine->Render(static_cast<King::Engine::Texture>(King::Engine::TEXTURE_TBD)
			, grid->gridXStart + gemCoordinade.first * grid->gridOffset + gridArray[gemCoordinade.first][gemCoordinade.second]->GetOffsetX()
			, grid->gridYStart + gemCoordinade.second * grid->gridOffset + gridArray[gemCoordinade.first][gemCoordinade.second]->GetOffsetY());
	}	
}