#include "GemGrid.h"
#include "Common.h"
#include <random>
#include <algorithm>

// http://stackoverflow.com/questions/19665818/generate-random-numbers-using-c11-random-library

GemGrid::GemGrid()
{
	InitializeGrid();
	gravity = 3;

	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		columnOffsets.push_back(std::make_pair(-1, 0));
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


/// <summary> Initializes gem grid
/// </summary>
void GemGrid::InitializeGrid()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			// Fill in by random values between 1 and 5 -> textures for gems
			while (true)
			{
				King::Engine::Texture texture = GenerateRandomGemColor();

				// Do not allow hits on the first generation
				if (!isInCascadeInitial(i, j, texture))
				{
					gemGrid[i][j] = new Gem(static_cast<King::Engine::Texture>(texture));
					break;
				}
			}	
		}
	}
}

/// <summary> Gets gem grid </summary>
/// <returns> gemGrid8x8 - Gem Grid </returns>
gemGrid8x8& GemGrid::GetGemGrid()
{
	return gemGrid;
}

/// <summary> Generates random color for gem
/// <returns> Random gem texture </returns>
/// </summary>
King::Engine::Texture GemGrid::GenerateRandomGemColor() const
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(1, std::nextafter(5, INT_MAX));

	return static_cast<King::Engine::Texture>(dist(mt));
}

/// <summary> Checks if there are matching 3-4-5 gems inside the grid
/// <returns> 
///			  Returns: True  - Match is present,   
///	                   False - Match is not present   
/// </returns>
/// </summary>
bool GemGrid::IsCascadePresent()
{
	// Check for mathes only if we are currenty not destroying gems
	if (!destructmentInProgress())
	{
		// Find matching gems
		findMatches();

		if(!IsGravityActive() && gemsToDestroy.size() > 0)
			ActivateGravity();
	}

	if (gemsToDestroy.size() > 0) return true;
	else return false;
}

/// <summary> Destroys gems </summary>
/// <returns> Number of gems destroyed </returns>
int GemGrid::DestroyGems()
{
	markToDestroy();

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
					gemGrid[column][row]->SetGemColor(GenerateRandomGemColor());
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

					// First element in the column should point to the one being destroyed
					gemGrid[column][0] = gemToDestroy;
					gemGrid[column][0]->SetGemColor(GenerateRandomGemColor());
				}
			}
		}
	}
	int score = gemsToDestroy.size();
	gemsToDestroy.clear();

	return score;
}

/// <summary> Gets gems which are meant to be destroyed </summary>
std::vector<std::pair<int, int>> GemGrid::GetGemsToDestroy()
{
	return gemsToDestroy;
}

/// <summary> Gets y offset of all the columns </summary>
std::vector<std::pair<int, int>> GemGrid::GetColumnOffsets()
{
	return columnOffsets;
}

/// <summary> Gets y offset of specific column </summary>
int GemGrid::GetColumnOffset(int column)
{
	return columnOffsets.at(column).second;
}

/// <summary> Let the nature do it's job with the gems </summary>
void GemGrid::ActivateGravity()
{
	// Disable user interaction while gravity is on 
	LockGrid();

	for (size_t iterator = 0; iterator < columnOffsets.size(); iterator++)
	{
		columnOffsets[iterator].first = 0;
		columnOffsets[iterator].second = 0;
	}

	std::sort(gemsToDestroy.begin(), gemsToDestroy.end(), [](auto &left, auto &right)
	{
		return (((left.first < right.first) && (left.second < right.second)));
	});

	for (auto gem : gemsToDestroy)
	{
		if (columnOffsets.at(gem.first).first < gem.second)
			columnOffsets.at(gem.first).first = gem.second;
		columnOffsets.at(gem.first).second -= static_cast<int>(gridOffset);
	}
}

/// <summary> Pull the gems to bottom </summary>
void GemGrid::GravityPull()
{
	bool isActive = false;
	for (size_t indexer = 0; indexer < columnOffsets.size(); ++indexer)
	{
		if (GetColumnOffset(indexer) < 0)
		{
			isActive = true;
			columnOffsets.at(indexer).second += gravity;
		}
	}

	if (!isActive)
	{
		// Enable user interaction if gravity did it's job
		UnlockGrid();
	}
}

/// <summary> Check if gravity is active </summary>
bool GemGrid::IsGravityActive()
{
	for (auto offset : columnOffsets)
	{
		if (offset.second < 0)
		{
			return true;
		}
	}
	return false;
}

/// <summary> Checks if the grid is locked for interaction </summary>
bool GemGrid::IsGridLocked()
{
	return gridLocked;
}

/// <summary> Locks grid for interaction </summary>
void GemGrid::LockGrid()
{
	gridLocked = true;
}

/// <summary> Unlocks grid for interaction </summary>
void GemGrid::UnlockGrid()
{
	gridLocked = false;
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

/// <summary> Checks if there are matching 3-4-5 gems to left and top.
/// This is used while gem grid is being generated, to prevent matches on game start.
/// <returns> 
///			  Returns: True  - Match is present,   
///	                   False - Match is not present   
/// </returns>
/// </summary>
bool GemGrid::isInCascadeInitial(int x, int y, King::Engine::Texture color)
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

/// <summary> Marks gems for destruction </summary>
void GemGrid::markToDestroy()
{

	for (auto pair : gemsToDestroy)
	{
		gemGrid[pair.first][pair.second]->SetToBeDestroyed(true);
	}
}

/// <summary> Check if gems are currenty marked for destruction </summary>
bool GemGrid::destructmentInProgress()
{
	if (gemsToDestroy.size() == 0) return false;
	else return true;
}

/// <summary> Finds current matches </summary>
void GemGrid::findMatches()
{
	findMatchesHorizontal();
	findMatchesVertical();
}

/// <summary> Finds current matches horizontally</summary>
void GemGrid::findMatchesHorizontal()
{
	// Go through the grid row by row in search for matches
	for (int row = 0; row < GRID_HEIGHT; ++row)
	{
		King::Engine::Texture color = gemGrid[0][row]->GetGemColor();
		int matchingGemsCount = 1;

		for (int column = 1; column < GRID_WIDTH; ++column)
		{
			// If current color is the same as the previous, increment match count
			if (color == gemGrid[column][row]->GetGemColor())
			{
				++matchingGemsCount;

				if (column == GRID_WIDTH - 1)
				{
					if (matchingGemsCount >= 3)
					{
						for (int columnOffset = 0; columnOffset < matchingGemsCount; columnOffset++)
						{
							// Do not add the gem if it is already added to destruction list
							if (!alreadyMarkedForDestruction(column - columnOffset, row))
							{
								gemsToDestroy.push_back(std::make_pair(column - columnOffset, row));
							}
						}
					}

					matchingGemsCount = 1;
				}
			}

			// Current game has different color than the previous one, or is the last one in the row
			if (color != gemGrid[column][row]->GetGemColor() || column == GRID_WIDTH - 1)
			{
				if (matchingGemsCount >= 3)
				{
					for (int columnOffset = 1; columnOffset <= matchingGemsCount; columnOffset++)
					{
						// Do not add the gem if it is already added to destruction list
						if (!alreadyMarkedForDestruction(column - columnOffset, row))
						{
							gemsToDestroy.push_back(std::make_pair(column - columnOffset, row));
						}
					}
				}
				color = gemGrid[column][row]->GetGemColor();
				matchingGemsCount = 1;
			}
		}
	}
}

/// <summary> Finds current matches vertically</summary>
void GemGrid::findMatchesVertical()
{
	// Go through the grid column by column in search for matches
	for (int column = 0; column < GRID_HEIGHT; ++column)
	{
		King::Engine::Texture color = gemGrid[column][0]->GetGemColor();
		int matchingGemsCount = 1;

		for (int row = 1; row < GRID_WIDTH; ++row)
		{
			if (color == gemGrid[column][row]->GetGemColor())
			{
				++matchingGemsCount;

				if (row == GRID_HEIGHT - 1)
				{
					if (matchingGemsCount >= 3)
					{
						for (int rowOffset = 0; rowOffset < matchingGemsCount; rowOffset++)
						{
							// Do not add the gem if it is already added to destruction list
							if (!alreadyMarkedForDestruction(column, row - rowOffset))
							{
								gemsToDestroy.push_back(std::make_pair(column, row - rowOffset));
							}
						}
					}

					matchingGemsCount = 1;
				}
			}

			// Current game has different color than the previous one, or is the last one in the column
			if (color != gemGrid[column][row]->GetGemColor())
			{
				if (matchingGemsCount >= 3)
				{
					for (int rowOffset = 1; rowOffset <= matchingGemsCount; rowOffset++)
					{
						// Do not add the gem if it is already added to destruction list
						if (!alreadyMarkedForDestruction(column, row - rowOffset))
						{
							gemsToDestroy.push_back(std::make_pair(column, row - rowOffset));
						}
					}
				}
				color = gemGrid[column][row]->GetGemColor();
				matchingGemsCount = 1;
			}
		}
	}
}

/// <summary> Checks if gem on position x,y is already selected for destruction </summary>
/// <returns> Boolean - is gem already marked for destruction </returns>
bool GemGrid::alreadyMarkedForDestruction(int column, int row)
{
	bool alreadyExists = false;
	for (auto gem : gemsToDestroy)
	{
		if (gem.first == column && gem.second == row)
		{
			alreadyExists = true;
		}
	}
	return alreadyExists;
}

/// <summary>
/// Starts moving process by setting coorinates of gem which should be moved and gem
/// toward which the gem is moved
/// </summary>
void GemGrid::TriggerGemMoving(int passedFromX, int passedFromY, int passedToX, int passedToY)
{
	LockGrid();
	gemMoving = true;
	fromX = passedFromX;
	fromY = passedFromY;
	toX = passedToX;
	toY = passedToY;
}

/// <summary>
/// Performs the action of moving gems. When gems are moved to maximal distance, switch their place.
/// </summary>
void GemGrid::MoveGem()
{
	// Go right
	if (fromX < toX)
	{
		// Napisi posebne funckije za kretanje livo desto itd..	
		gemGrid[fromX][fromY]->MoveRight();
		gemGrid[toX][toY]->MoveLeft();

		// Came to the other side -> switch places
		if (gemGrid[fromX][fromY]->GetOffsetX() >= gridOffset)
		{
			SwitchGems();
		}
	}
	// Go left
	else if (fromX > toX)
	{
		gemGrid[fromX][fromY]->MoveLeft();
		gemGrid[toX][toY]->MoveRight();

		// Came to the other side -> switch places
		if (gemGrid[fromX][fromY]->GetOffsetX() <= -gridOffset)
		{
			SwitchGems();
		}
	}
	// Go down
	else if (fromY < toY)
	{
		gemGrid[fromX][fromY]->MoveDown();
		gemGrid[toX][toY]->MoveUp();

		// Came to the other side -> switch places
		if (gemGrid[fromX][fromY]->GetOffsetY() >= gridOffset)
		{
			SwitchGems();
		}
	}
	// Go up
	else if (fromY > toY)
	{
		gemGrid[fromX][fromY]->MoveUp();
		gemGrid[toX][toY]->MoveDown();

		// Came to the other side -> switch places
		if (gemGrid[fromX][fromY]->GetOffsetY() <= -gridOffset)
		{
			SwitchGems();
		}
	}

	
}

/// <summary> Switches places of two gems depending on their coordinates 
///			  stored in gemGrid object 
/// </summary>
void GemGrid::SwitchGems()
{
	Gem* switchingGem;
	switchingGem = gemGrid[toX][toY];
	gemGrid[toX][toY] = gemGrid[fromX][fromY];
	gemGrid[fromX][fromY] = switchingGem;
	gemGrid[fromX][fromY]->ResetOffset();
	gemGrid[toX][toY]->ResetOffset();
	UnlockGrid();
	gemMoving = false;
}

