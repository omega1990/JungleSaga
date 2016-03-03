#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>

#include "GemGrid.h"
#include "Game.h"


int main(int argc, char *argv[]) {
	GemGrid* grid = new GemGrid();
	Game game(*grid);

	// Test method
	grid->PrintGrid();
	
	game.Start();


	return 0;
}


