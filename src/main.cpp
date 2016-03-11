#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define GLM_FORCE_RADIANS 

#include "Game.h"


int main(int argc, char *argv[]) 
{	
	Game* game = new Game();	

	game->Start();

	return 0;
}


