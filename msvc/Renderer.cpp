#include "Renderer.h"

Renderer::Renderer(King::Engine& passedEngine, GemGrid& passedGrid)
	: mEngine(&passedEngine)
	, grid(&passedGrid)
	, clock(0.0f)
	, matchRendererInProgress(false)
{
}

Renderer::~Renderer()
{
}

/// <summary> Renders start screen </summary>
void Renderer::RenderStartScreen(float x, float y) const
{
	mEngine->Render(King::Engine::TEXTURE_START, x, y);
}

/// <summary> Renders background </summary>
void Renderer::RenderBackground() const
{
	mEngine->Render(King::Engine::TEXTURE_BACKGROUND, 0, 0);
}

/// <summary> Renders top of the gem grid </summary>
void Renderer::RenderTop() const
{
	mEngine->Render(King::Engine::TEXTURE_TOP, 83, 0);
}

/// <summary> Renders score </summary>
void Renderer::RenderScore(const char* score) const
{
	mEngine->Write(score, 80.0f, 260.0f);
}

/// <summary> Renders score </summary>
void Renderer::RenderTime(const char* time) const
{
	mEngine->Write(time, 645.0f, 260.0f);
}

/// <summary> Renders gem grid </summary>
void Renderer::RenderGemGrid() const
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
	matchRendererInProgress = true;

	// If more than 0.02 seconds passed
	if (clock > 0.02f)
	{
		clock = 0.0f;
		matchRendererInProgress = false;
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
void Renderer::RenderSelected(int selectedGemX, int selectedGemY) const
{
	if (selectedGemX != -1 && selectedGemY != -1)
		mEngine->Render(King::Engine::TEXTURE_SELECTED, grid->gridXStart + selectedGemX*grid->gridOffset - 4.0f, grid->gridYStart + selectedGemY*grid->gridOffset - 4.0f);
}

/// <summary> Renders menu screen </summary>
void Renderer::RenderMenu() const
{
	RenderStartScreen();
	mEngine->Write("Click to start", 290.0f, 560.0f);
}

/// <summary> Renders sliding menu screen </summary>
void Renderer::RenderMenuSlide(float x, float y) const
{
	RenderBackground();
	RenderStartScreen(x, y);
}

/// <summary> Renders game over sliding screen </summary>
void Renderer::RenderGameOverSlide(float x, float y) const
{
	RenderBackground();
	RenderStartScreen(x, y);
}

/// <summary> Renders game over screen </summary>
void Renderer::RenderGameOver(const char* score) const
{
	RenderStartScreen();
	mEngine->Write("Score:", 325.0f, 530.0f);
	mEngine->Write(score, 410.0f, 530.0f);
	mEngine->Write("Click to retry", 295.0f, 560.0f);
}

/// <summary> Check if rendering of current match is ongoing </summary>
bool Renderer::IsMatchRenderingInProgress() const
{
	return matchRendererInProgress;
}