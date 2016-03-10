#include "Renderer.h"

Renderer::Renderer(King::Engine& passedEngine, GemGrid& passedGrid)
	: mEngine(&passedEngine)
	, grid(&passedGrid)
{
}

Renderer::~Renderer()
{
}

/// <summary> Renders start screen </summary>
void Renderer::RenderStartScreen(float x, float y)
{
	mEngine->Render(King::Engine::TEXTURE_START, x, y);
}

/// <summary> Renders background </summary>
void Renderer::RenderBackground()
{
	mEngine->Render(King::Engine::TEXTURE_BACKGROUND, 0, 0);
}

/// <summary> Renders top of the gem grid </summary>
void Renderer::RenderTop()
{
	mEngine->Render(King::Engine::TEXTURE_TOP, 214, 0);
}

/// <summary> Renders gem grid </summary>
void Renderer::RenderGemGrid()
{
	gemGrid8x8& gemGrid = grid->GetGemGrid();

	// Draw complete gem grid
	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		for (int j = 0; j < GRID_HEIGHT; ++j)
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

			mEngine->Render(static_cast<King::Engine::Texture>(gemGrid[i][j]->GetGemColor())
				, grid->gridXStart + i * grid->gridOffset + gemGrid[i][j]->GetOffsetX()
				, grid->gridYStart + j * grid->gridOffset + gemGrid[i][j]->GetOffsetY() + offset);
		}
	}
}

/// <summary> Renders which gems are going to be destroyed </summary>
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

	gemGrid8x8& gemGrid = grid->GetGemGrid();

	for (auto gemCoordinade : gemsToBeDestroyed)
	{
		mEngine->Render(static_cast<King::Engine::Texture>(King::Engine::TEXTURE_TBD)
			, grid->gridXStart + gemCoordinade.first * grid->gridOffset + gemGrid[gemCoordinade.first][gemCoordinade.second]->GetOffsetX()
			, grid->gridYStart + gemCoordinade.second * grid->gridOffset + gemGrid[gemCoordinade.first][gemCoordinade.second]->GetOffsetY());
	}


}

/// <summary> Renders red square around selected gem </summary>
void Renderer::RenderSelected(int selectedGemX, int selectedGemY)
{
	if (selectedGemX != -1 && selectedGemY != -1)
		mEngine->Render(King::Engine::TEXTURE_SELECTED, grid->gridXStart + selectedGemX*grid->gridOffset - 4.0f, grid->gridYStart + selectedGemY*grid->gridOffset - 4.0f);
}

/// <summary> Renders menu screen </summary>
void Renderer::RenderMenu()
{
	RenderStartScreen();
	mEngine->Write("Click to start", 290.0f, 560.0f);
}

/// <summary> Renders sliding menu screen </summary>
void Renderer::RenderMenuSlide(float x, float y)
{
	RenderBackground();
	RenderStartScreen(x, y);
}

/// <summary> Renders game over sliding screen </summary>
void Renderer::RenderGameOverSlide(float x, float y)
{
	RenderBackground();
	RenderStartScreen(x, y);
}

/// <summary> Renders game over screen</summary>
void Renderer::RenderGameOver(const char* score)
{
	RenderStartScreen();
	mEngine->Write("Score:", 330.0f, 530.0f);
	mEngine->Write(score, 410.0f, 530.0f);
	mEngine->Write("Click to retry", 295.0f, 560.0f);
}