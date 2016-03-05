#include "GemGrid.h"
#include "Common.h"
#include <random>


// http://stackoverflow.com/questions/19665818/generate-random-numbers-using-c11-random-library

GemGrid::GemGrid()
{
	InitializeGrid();


	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		pullValues.push_back(std::make_pair(i, 0));
	}
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
	MarkToDestroy(gemsToDestroy); 

	for (int row = 0; row < GRID_WIDTH; ++row)
	{
		for (int column = 0; column < GRID_HEIGHT; ++column)
		{
			gemGrid[column][row]->ResetOffset();
			if (gemGrid[column][row]->GetToBeDestroyed())
			{
				gemGrid[column][row]->SetToBeDestroyed(false);
				if (row == 0)
				{
					gemGrid[column][row]->SetGemColor(GenerateRandomTexture());
				}
				else
				{
					int currentRow = row;
					Gem* gemToDestroy = gemGrid[column][row];

					while (currentRow > 0)
					{
						gemGrid[column][currentRow] = gemGrid[column][currentRow - 1];
						--currentRow;
					}

					// Pridjeli objekt koji se unistava prvome u stupcu
					gemGrid[column][0] = gemToDestroy;
					gemGrid[column][0]->SetGemColor(GenerateRandomTexture());
				}
			}
		}
	}
}

void GemGrid::MarkToDestroy(std::vector<std::pair<int, int>> gemsToDestroy)
{

	for (auto pair : gemsToDestroy)
	{
		gemGrid[pair.first][pair.second]->SetToBeDestroyed(true);
	}
}

bool GemGrid::IsGridLocked()
{
	return gridLocked;
}

void GemGrid::LockGrid()
{
	gridLocked = true;
}

void GemGrid::UnlockGrid() 
{
	gridLocked = false;
}


//
//void GemGrid::ActivateGravity(std::vector<std::pair<int, int>> gemsToDestroy)
//{
//	for (int row = 0; row < GRID_WIDTH; ++row)
//	{
//		for (int column = 0; column < GRID_HEIGHT; ++column)
//		{
//			if (gemGrid[column][row]->GetGemColor() == King::Engine::TEXTURE_EMPTY)
//			{
//				int currentRow = row - 1;
//				while (currentRow >= 0)
//				{
//					if (gemGrid[column][currentRow]->GetGemColor() != King::Engine::TEXTURE_EMPTY)
//						gemGrid[column][currentRow]->SetGravity(true);
//					--currentRow;
//				}
//			}
//		}
//	}
//}
//
//bool GemGrid::GravityPull(std::vector<std::pair<int, int>> gemsToDestroy)
//{
//	bool pullEnd = false;
//	for (int row = 0; row < GRID_WIDTH; ++row)
//	{
//		for (int column = 0; column < GRID_HEIGHT; ++column)
//		{
//			if (gemGrid[column][row]->GetGravity())
//			{
//				int maxOffset = 0;
//
//				for (auto gem : gemsToDestroy)
//				{
//					if (gem.first == column && gem.second >= row)
//					{
//						++maxOffset;
//					}
//				}
//
//				gemGrid[column][row]->SetGravityOffsetY(gemGrid[column][row]->GetOffsetY() + 3.0f);
//				if (gemGrid[column][row]->GetOffsetY() >= gridOffset*maxOffset)
//				{
//					gemGrid[column][row]->SetGravity(false);
//					pullEnd = true;
//				}
//			}
//			else if (gemGrid[column][row]->GetGemColor() == King::Engine::TEXTURE_EMPTY &&
//				row == 0)
//			{
//				pullEnd = true;
//			}
//		}
//	}
//	return pullEnd;
//}