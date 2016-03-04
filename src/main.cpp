#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>

#include "GemGrid.h"
#include "Game.h"


int main(int argc, char *argv[]) 
{	
	Game* game = new Game();	

	game->Start();

	return 0;
}


