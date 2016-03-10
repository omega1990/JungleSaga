#include "GemGrid.h"


GemGrid::GemGrid()
	: gravityIncrementer(0.0f)
	, switchBack(false)
{
	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		columnOffsets.push_back(std::make_pair(DEFAULT_COORDINATE, 0.0f));
	}
	InitializeGrid();
}

GemGrid::~GemGrid()
{
	// Free memory allocated on heap for gem grid
	EmptyGrid();
}

/// <summary> Initializes gem grid </summary>
void GemGrid::InitializeGrid(bool reshuffle)
{
	LockGrid();
	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		// Reshuffle will not trigger falling of gems from the top
		if (!reshuffle)
		{
			columnOffsets.at(i).first = GRID_HEIGHT - 1;
			columnOffsets.at(i).second -= static_cast<float>((GRID_HEIGHT * gridOffset + (GRID_HEIGHT - i) * gridOffset));
		}
		for (int j = 0; j < GRID_HEIGHT; ++j)
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
	checkPossibleMoves = true;
}

/// <summary> Frees memory allocated for gem grid </summary>
void GemGrid::EmptyGrid()
{
	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		for (int j = 0; j < GRID_HEIGHT; ++j)
		{
			delete gemGrid[i][j];
		}
	}
}

/// <summary> Generate new gems in gem grid </summary>
void GemGrid::Reshuffle()
{
	EmptyGrid();
	InitializeGrid(true);
}

/// <summary> Gets gem grid </summary>
gemGrid8x8& GemGrid::GetGemGrid()
{
	return gemGrid;
}

/// <summary> Generates random color for gem </summary>
King::Engine::Texture GemGrid::GenerateRandomGemColor() const
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(1.0, std::nextafter(5, INT_MAX));

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
	if (!destructionPending())
	{
		// Find matching gems
		findMatches(false);
	}

	return destructionPending();
}

/// <summary> Destroys gems 
/// <returns> Number of gems destroyed </returns>
/// </summary>
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

	checkPossibleMoves = true;

	return score;
}

/// <summary> Gets gems which are meant to be destroyed </summary>
std::vector<std::pair<int, int>> GemGrid::GetGemsToDestroy() const
{
	return gemsToDestroy;
}

/// <summary> Gets y offset of all the columns </summary>
std::vector<std::pair<int, float>> GemGrid::GetColumnOffsets() const
{
	return columnOffsets;
}

/// <summary> Gets y offset of specific column </summary>
float GemGrid::GetColumnOffset(int column) const
{
	return columnOffsets.at(column).second;
}

/// <summary> Let the nature do it's job with the gems </summary>
void GemGrid::ActivateGravity()
{
	// Disable user interaction while gravity is on 
	LockGrid();

	// Reset all active offsets
	for (size_t iterator = 0; iterator < columnOffsets.size(); iterator++)
	{
		columnOffsets[iterator].first = 0;
		columnOffsets[iterator].second = 0.0f;
	}

	std::sort(gemsToDestroy.begin(), gemsToDestroy.end(), [](auto &left, auto &right)
	{
		return (((left.first < right.first) && (left.second < right.second)));
	});

	// Launch gems into the air so the gravity can do it's job
	for (auto gem : gemsToDestroy)
	{
		if (columnOffsets.at(gem.first).first < gem.second)
			columnOffsets.at(gem.first).first = gem.second;
		columnOffsets.at(gem.first).second -= gridOffset;
	}
}

/// <summary> Pull the gems to bottom </summary>
void GemGrid::GravityPull()
{
	for (size_t indexer = 0; indexer < columnOffsets.size(); ++indexer)
	{
		if (GetColumnOffset(indexer) < 0.0f)
		{
			columnOffsets.at(indexer).second += (gravity + gravityIncrementer);			
		}
	}
	gravityIncrementer += 0.2f;

	if (!IsGravityActive())
	{
		// Enable user interaction if gravity did it's job
		gravityIncrementer = 0.0f;
		UnlockGrid();
	}
}

/// <summary> Check if gravity is active </summary>
bool GemGrid::IsGravityActive() const
{
	for (auto offset : columnOffsets)
	{
		// Pull has pulled the gems to the bottom
		if (offset.second < 0.0f)
		{
			return true;
		}
	}
	return false;
}

/// <summary> Checks if the grid is locked for interaction </summary>
bool GemGrid::IsGridLocked() const
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
void GemGrid::PrintGrid() const
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
bool GemGrid::destructionPending()
{
	if (gemsToDestroy.empty()) return false;
	else return true;
}

/// <summary> Finds current matches </summary>
bool GemGrid::findMatches(bool check)
{
	if (check)
	{
		if (findMatchesHorizontal(check)) return true;
		if (findMatchesVertical(check)) return true;
	}
	else
	{
		findMatchesHorizontal(false);
		findMatchesVertical(false);
	}
}

/// <summary> Finds current matches horizontally</summary>
bool GemGrid::findMatchesHorizontal(bool check)
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
						if (check)
						{
							gemsToDestroy.clear();
							return true;
						}
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
					if (check)
					{
						gemsToDestroy.clear();
						return true;
					}
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
	return false;
}

/// <summary> Finds current matches vertically </summary>
bool GemGrid::findMatchesVertical(bool check)
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
						if (check)
						{
							gemsToDestroy.clear();
							return true;
						}
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
					if (check)
					{
						gemsToDestroy.clear();
						return true;
					}
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
	return false;
}

// Used in find algorithm for temporary switching
void GemGrid::switchAndCheckMatches(int x1, int y1, int x2, int y2)
{
	// Swap gem and see if this results in a match
	Gem* tempGem;
	tempGem = gemGrid[x1][y1];
	gemGrid[x1][y1] = gemGrid[x2][y2];
	gemGrid[x2][y2] = tempGem;

	// If it is a match, put it in the possible moves vector
	if (findMatches(true))
	{
		possibleMoves.push_back(std::make_pair(std::make_pair(x1, y1), std::make_pair(x2, y2)));
	}

	tempGem = gemGrid[x2][y2];
	gemGrid[x2][y2] = gemGrid[x1][y1];
	gemGrid[x1][y1] = tempGem;
}

/// <summary> Finds all possible moves at the moment </summary>
possibleMoves GemGrid::FindPossibleMoves()
{
	possibleMoves.clear();

	// o o o o o o o 
	// ^
	for (int row = 0; row < GRID_HEIGHT; ++row)
	{
		for (int column = 0; column < GRID_WIDTH; ++column)
		{
			if (row > 0)
			{
				switchAndCheckMatches(row, column, row - 1, column);
			}

			if (row < GRID_HEIGHT - 1)
			{
				switchAndCheckMatches(row, column, row + 1, column);
			}

			if (column > 0)
			{
				switchAndCheckMatches(row, column, row, column - 1);
			}

			if (column < GRID_WIDTH - 1)
			{
				switchAndCheckMatches(row, column, row, column + 1);
			}
		}
	}

	// Whem this is performed once, do not execute this until there are some gems destroyed
	checkPossibleMoves = false;

	return possibleMoves;
}


/// <summary> Checks if gem on position x,y is already selected for destruction 
/// <returns> Boolean - is gem already marked for destruction </returns>
/// </summary>
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
void GemGrid::SwitchGems(int passedFromX, int passedFromY, int passedToX, int passedToY)
{
	gemMoving = true;
	LockGrid();
	fromX = passedFromX;
	fromY = passedFromY;
	toX = passedToX;
	toY = passedToY;
	AnimateGemSwitch();
}

/// <summary>
/// Performs the action of moving gems. When gems are moved to maximal distance, switch their place.
/// </summary>
void GemGrid::AnimateGemSwitch()
{
	bool movePossible = false;
	LockGrid();
	for (auto possibleMove : possibleMoves)
	{
		if (possibleMove.first.first == fromX && possibleMove.first.second == fromY
			&& possibleMove.second.first == toX && possibleMove.second.second == toY)
		{
			movePossible = true;
		}
	}

	// Go right
	if (fromX < toX)
	{
		gemGrid[fromX][fromY]->MoveRight();
		gemGrid[toX][toY]->MoveLeft();

		// Came to the other side -> switch places
		if (gemGrid[fromX][fromY]->GetOffsetX() >= gridOffset)
		{
			SwitchGemsPositions();
			// If move is not valid, switch the gem back
			if (!movePossible && !switchBack)
			{
				switchBack = true;
				// By setting switchBack to true, gems will not be switched again here
				SwitchGems(toX, toY, fromX, fromY);
			}
			else
			{
				switchBack = false;
				gemMoving = false;
				UnlockGrid();
			}
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
			SwitchGemsPositions();
			if (!movePossible && !switchBack)
			{
				switchBack = true;
				// If we call the function with switchBack set to true, gems will not be switched again here
				SwitchGems(toX, toY, fromX, fromY);
			}
			else
			{
				switchBack = false;
				gemMoving = false;
				UnlockGrid();
			}
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
			SwitchGemsPositions();
			if (!movePossible && !switchBack)
			{
				switchBack = true;
				// If we call the function with switchBack set to true, gems will not be switched again here
				SwitchGems(toX, toY, fromX, fromY);
			}
			else
			{
				switchBack = false;
				gemMoving = false;
				UnlockGrid();
			}
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
			SwitchGemsPositions();
			if (!movePossible && !switchBack)
			{
				switchBack = true;
				// If we call the function with switchBack set to true, gems will not be switched again here
				SwitchGems(toX, toY, fromX, fromY);
			}
			else
			{
				switchBack = false;
				gemMoving = false;
				UnlockGrid();
			}
		}
	}
}

/// <summary> Switches places of two gems depending on their coordinates 
///			  stored in gemGrid object 
/// </summary>
void GemGrid::SwitchGemsPositions()
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


/// <summary> Check if gem is currently moving </summary>
bool GemGrid::IsGemMoving() const
{
	return gemMoving;
}

/// <summary> Declare that one of gems is currently moving </summary>
void GemGrid::SetGemMoving(bool isMoving)
{
	gemMoving = isMoving;
}

/// <summary> Check if checking of possible moves needs to be performed </summary>
bool GemGrid::possibleCheckPending() const
{
	return checkPossibleMoves;
}
