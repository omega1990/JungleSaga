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

void Renderer::RenderGemGrid(int x, int y, float offsetX, float offsetY, direction dir)
{
	int x1 = 0;
	int y1 = 0;

	switch (dir)
	{
	case HORIZONTAL_LEFT:
		x1 = x - 1;
		y1 = y;
		break;
	case HORIZONTAL_RIGHT:
		x1 = x + 1;
		y1 = y;
		break;
	case VERTICAL_UP:
		x1 = x;
		y1 = y + 1;
		break;
	case VERTICAL_DOWN:
		x1 = x;
		y1 = y - 1;
		break;
	case STATIONARY:
		// If we were moving horizontally
		if (offsetX != 0)
		{
			if (offsetX > 0)
			{
				x1 = x + 1;
				y1 = y;
			}
			else 
			{
				x1 = x - 1;
				y1 = y;
			}
		}
		else
		{
			// If we were moving vertically
			if (offsetY > 0)
			{
				x1 = x;
				y1 = y + 1;
			}
			else
			{
				x1 = x;
				y1 = y - 1;
			}
		}
		break;
	}

	int** gridArray = grid->getGemGrid();
	// Draw complete gem grid
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if ((i == x &&  j == y) || (i == x1 && j == y1))
			{
				// Draw current element the last
				continue;
			}
			else
			{
				mEngine->Render(static_cast<King::Engine::Texture>(gridArray[i][j])
					, grid->gridXStart + i * grid->gridOffset
					, grid->gridYStart + j * grid->gridOffset);
			}
		}
	}

	mEngine->Render(static_cast<King::Engine::Texture>(gridArray[x1][y1])
		, grid->gridXStart + x1 * grid->gridOffset - offsetX
		, grid->gridYStart + y1 * grid->gridOffset - offsetY);

	mEngine->Render(static_cast<King::Engine::Texture>(gridArray[x][y])
		, grid->gridXStart + x * grid->gridOffset + offsetX
		, grid->gridYStart + y * grid->gridOffset + offsetY);
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