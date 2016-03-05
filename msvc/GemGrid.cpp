#include "GemGrid.h"
#include "Common.h"
#include <random>


// http://stackoverflow.com/questions/19665818/generate-random-numbers-using-c11-random-library

GemGrid::GemGrid()
{
	InitializeGrid();
}

GemGrid::~GemGrid()
{
	// Destroy dinamically allocated objects
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			delete gemGrid[i][j];
		}
	}
}

void GemGrid::InitializeGrid()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			// Fill in by random values between 1 and 5 -> textures for gems
			while (true)
			{
				King::Engine::Texture texture = GenerateRandomTexture();

				// Do not allow hits on the first generation
				if (!IsInCascadeInitial(i, j, texture))
				{
					gemGrid[i][j] = new Gem(static_cast<King::Engine::Texture>(texture));
					break;
				}
			}
		}
	}
}

King::Engine::Texture GemGrid::GenerateRandomTexture() const
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(1, std::nextafter(5, INT_MAX));

	return static_cast<King::Engine::Texture>(dist(mt));
}

// Check left and top
bool GemGrid::IsInCascadeInitial(int x, int y, King::Engine::Texture color)
{
	if (x > 1)
	{
		if (color == gemGrid[x - 1][y]->GetGemColor() &&
			color == gemGrid[x - 2][y]->GetGemColor())
		{
			return true;
		}
	}
	if (y > 1)
	{
		if (color == gemGrid[x][y - 1]->GetGemColor() &&
			color == gemGrid[x][y - 2]->GetGemColor())
		{
			return true;
		}
	}

	return false;
}

// Check left and top
std::vector<std::pair<int, int>> GemGrid::IsCascadePresent()
{
	int hitCount = 0;

	std::vector<std::pair<int, int>> gemsToDestroy;

	King::Engine::Texture color;

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (j == 0)
			{
				color = gemGrid[i][j]->GetGemColor();
			}
			else
			{
				if (color == gemGrid[i][j]->GetGemColor())
				{
					hitCount++;
					std::cout << hitCount << std::endl;
				}
				else
				{
					if (hitCount < 3)
					{
						hitCount = 1;
					}
					else
					{
						for (int p = 1; p <= hitCount; ++p)
						{
							gemsToDestroy.push_back(std::make_pair(i, j - p));
						}
						hitCount = 1;
					}
					color = gemGrid[i][j]->GetGemColor();
				}
			}
		}
		if (hitCount < 3)
		{
			hitCount = 1;
		}
		else
		{
			for (int p = 0; p < hitCount; ++p)
			{
				gemsToDestroy.push_back(std::make_pair(i, 7 - p));
			}
			hitCount = 1;
		}
	}

	hitCount = 1;

	for (int j = 0; j < 8; ++j)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (i == 0)
			{
				color = gemGrid[i][j]->GetGemColor();
			}
			else
			{
				if (color == gemGrid[i][j]->GetGemColor())
				{
					hitCount++;
					std::cout << hitCount << std::endl;
				}
				else
				{
					if (hitCount < 3)
					{
						hitCount = 1;
					}
					else
					{
						for (int p = 1; p <= hitCount; ++p)
						{
							gemsToDestroy.push_back(std::make_pair(i - p, j));
						}
						hitCount = 1;
					}
					color = gemGrid[i][j]->GetGemColor();
				}
			}
		}
		if (hitCount < 3)
		{
			hitCount = 1;
		}
		else
		{
			for (int p = 0; p < hitCount; ++p)
			{
				gemsToDestroy.push_back(std::make_pair(7 - p, j));
			}
			hitCount = 1;
		}
	}

	return gemsToDestroy;
}

// Testing function
void GemGrid::PrintGrid()
{
	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		for (int j = 0; j < GRID_HEIGHT; ++j)
		{
			std::cout << gemGrid[i][j]->GetGemColor() << " ";
		}
		std::cout << std::endl;
	}
}

gemGrid8x8& GemGrid::getGemGrid()
{
	return gemGrid;
}


void GemGrid::DestroyGems(std::vector<std::pair<int, int>> gemsToDestroy)
{
	for (auto pair : gemsToDestroy)
	{
		gemGrid[pair.first][pair.second]->SetGemColor(King::Engine::TEXTURE_EMPTY);
	}

	/*for (int j = 0; j < 8; ++j)
	{
		for (int i = 0; i < 7; ++i)
		{
			if (gemGrid[i][j + 1]->GetGemColor() == King::Engine::TEXTURE_EMPTY)
			{
				int z = j;
				while (z != -1)
				{
					gemGrid[i][z + 1]->SetGemColor(gemGrid[i][z]->GetGemColor());
					--z;
				}
				gemGrid[i][0]->SetGemColor(GenerateRandomTexture());
			}
		}
	}*/

}