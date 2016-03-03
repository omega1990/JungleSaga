#include "Renderer.h"

Renderer::Renderer(King::Engine& passedEngine, GemGrid& passedGrid)
{
	mEngine = &passedEngine;
	grid = &passedGrid;
}

Renderer::~Renderer()
{
}

void Renderer::RenderBackground() 
{
	mEngine->Render(King::Engine::TEXTURE_BACKGROUND, 0, 0);
}

void Renderer::RenderGemGrid(int x, int y, float offsetX, float offsetY)
{
	int** gridArray = grid->getGemGrid();
	// Draw complete gem grid
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (i == x &&  j == y)
			{
				mEngine->Render(static_cast<King::Engine::Texture>(gridArray[i][j])
					, grid->gridXStart + i * grid->gridOffset + offsetX
					, grid->gridYStart + j * grid->gridOffset + offsetY);
			}
			else
			{
				mEngine->Render(static_cast<King::Engine::Texture>(gridArray[i][j])
					, grid->gridXStart + i * grid->gridOffset
					, grid->gridYStart + j * grid->gridOffset);
			}
		}
	}
}

void Renderer::RenderGemGrid()
{
	int** gridArray = grid->getGemGrid();
	// Draw complete gem grid
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			mEngine->Render(static_cast<King::Engine::Texture>(gridArray[i][j])
				, grid->gridXStart + i * grid->gridOffset
				, grid->gridYStart + j * grid->gridOffset);
		}
	}
}