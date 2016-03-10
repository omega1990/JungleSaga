#pragma once
#include<vector>

#define GRID_WIDTH 8
#define GRID_HEIGHT 8

#define MAX_GEM_OFFSET 43.0f
#define DEFAULT_COORDINATE -1

#define OFFSET_FOR_SWAP 15.0f
#define OFFSET_FOR_SWIPE 1.0f


typedef class Gem* (gemGrid8x8)[8][8];

typedef std::pair<std::pair<int, int>, std::pair<int, int>> possibleMove;
typedef std::vector<possibleMove> possibleMoves;

