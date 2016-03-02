#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>

#include "GemGrid.h"
//**********************************************************************

class ExampleGame : public King::Updater {
public:

	ExampleGame()
		: mEngine("./assets") // Pass the path to assets - pictures etc
		, mRotation(0.0f)
		, mYellowDiamondX(100.0f)
		, mYellowDiamondY(100.0f) {
	}

	void Start(GemGrid& passedGrid) {
		grid = &passedGrid;
		mEngine.Start(*this);
	}

	void Update() {
		// Draw grid
		int** gridArray = grid->getGemGrid();

		mEngine.Render(King::Engine::TEXTURE_BACKGROUND, 0, 0);
		mRotation += mEngine.GetLastFrameSeconds();

		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				switch (*(*(gridArray + i) + j))
				{
				case King::Engine::TEXTURE_GREEN:
					mEngine.Render(King::Engine::TEXTURE_GREEN, grid->gridXStart + i * grid->gridOffset, grid->gridYStart + j * grid->gridOffset);
					break;
				case King::Engine::TEXTURE_YELLOW:
					mEngine.Render(King::Engine::TEXTURE_YELLOW, grid->gridXStart + i * grid->gridOffset, grid->gridYStart + j * grid->gridOffset);
					break;
				case King::Engine::TEXTURE_PURPLE:
					mEngine.Render(King::Engine::TEXTURE_PURPLE, grid->gridXStart + i * grid->gridOffset, grid->gridYStart + j * grid->gridOffset);
					break;
				case King::Engine::TEXTURE_BLUE:
					mEngine.Render(King::Engine::TEXTURE_BLUE, grid->gridXStart + i * grid->gridOffset, grid->gridYStart + j * grid->gridOffset);
					break;
				case King::Engine::TEXTURE_RED:
					mEngine.Render(King::Engine::TEXTURE_RED, grid->gridXStart + i * grid->gridOffset, grid->gridYStart + j * grid->gridOffset);
					break;
				default:
					break;
				}
			}
		}


	
		//mEngine.Render(King::Engine::TEXTURE_BLUE, 650.0f, 450.0f);

		//mEngine.Write("Green", 650.0f, 140.0f);
		//mEngine.Write("Red", 100.0f, 490.0f);
		//mEngine.Write("Blue", 650.0f, 490.0f);

		//const char text[] = "This rotates at 5/PI Hz";
		//mRotation += mEngine.GetLastFrameSeconds();
		//mEngine.Write(text, mEngine.GetWidth() / 2.0f, mEngine.GetHeight() / 2.0f, mRotation * 20.5f);

		//if (mEngine.GetMouseButtonDown()) {
		//	mYellowDiamondX = mEngine.GetMouseX();
		//	mYellowDiamondY = mEngine.GetMouseY();
		//}
		//mEngine.Render(King::Engine::TEXTURE_YELLOW, mYellowDiamondX, mYellowDiamondY);
		//mEngine.Write("Click to", mYellowDiamondX, mYellowDiamondY + 40.0f);
		//mEngine.Write("move me!", mYellowDiamondX, mYellowDiamondY + 70.0f);		
		
		//mEngine.Write("Jelena", mEngine.GetWidth() / 2.0f, mEngine.GetHeight() / 2.0f, mRotation * 20.5f);
		
	}

private:
	King::Engine mEngine;
	GemGrid* grid;
	float mRotation;
	float mYellowDiamondX;
	float mYellowDiamondY;
};

//**********************************************************************

int main(int argc, char *argv[]) {
	ExampleGame game;
	GemGrid* grid = new GemGrid();

	grid->PrintGrid();
	
	game.Start(*grid);


	return 0;
}


