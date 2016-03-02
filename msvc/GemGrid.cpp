#include "GemGrid.h"
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
		delete[] gemGrid;
	}
}

void GemGrid::InitializeGrid()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(1, std::nextafter(5, INT_MAX));

	gemGrid = new int*[8];
	for (int i = 0; i < 8; ++i)
	{
		gemGrid[i] = new int[8];
		for (int j = 0; j < 8; ++j)
		{
			// Fill in by random values between 1 and 5 -> textures for gems
			while (true)
			{
				gemGrid[i][j] = dist(mt);
				// Do not allow hits on the first generation
				if (!IsInCascadeInitial(i, j)) break;
				else std::cout << "Cascade!!" << std::endl;
			}			
		}
	}
}

// Check left and top
bool GemGrid::IsInCascadeInitial(int x, int y)
{
	if (x > 1)
	{
		if (gemGrid[x][y] == gemGrid[x - 1][y] && 
			gemGrid[x][y] == gemGrid[x - 2][y])
		{
			return true;
		}
	}
	if (y > 1)
	{
		if (gemGrid[x][y] == gemGrid[x][y - 1] &&
			gemGrid[x][y] == gemGrid[x][y - 2])
		{
			return true;
		}
	}

	return false;
}

// Testing function
void GemGrid::PrintGrid()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			std::cout << *(*(gemGrid + i) + j) << " ";
		}
		std::cout << std::endl;
	}
}

int** GemGrid::getGemGrid()
{
	return gemGrid;
}
