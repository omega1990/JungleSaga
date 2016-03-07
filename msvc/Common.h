#pragma once

#define GRID_WIDTH 8
#define GRID_HEIGHT 8

#define MAX_GEM_OFFSET 43.0f

#include<vector>

// Forward declaration
typedef class Gem* (gemGrid8x8)[8][8];

typedef std::pair<std::pair<int, int>, std::pair<int, int>> possibleMove;
typedef std::vector<possibleMove> possibleMoves;

//using gemGrid8x8 = std::array<std::array<int*, 8>, 8>&;
