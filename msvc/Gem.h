#pragma once
#include <king\Engine.h>
#include <iostream>
#include "Common.h"

class Gem
{
public:
	Gem(King::Engine::Texture passedGemColor);
	~Gem();

	enum direction
	{
		HORIZONTAL_RIGHT,
		HORIZONTAL_LEFT,
		VERTICAL_UP,
		VERTICAL_DOWN,
		STATIONARY,
		OUT_OF_BOUNDS,
		TO_BE_SWITCHED
	};

	King::Engine::Texture GetGemColor();

	float mouseMoveStartX;
	float mouseMoveStartY;

	
	direction moveDirection;

	bool Selected = false;

	float GetOffset(direction dir, float mouseX, float mouseY);
	void SetOffsetX(float passedOffsetX);
	void SetOffsetY(float passedOffsetY);

	float GetOffsetX() { return offsetX; };
	float GetOffsetY() { return offsetY; };

	void ResetOffset();

private:
	King::Engine::Texture gemColor;
	float offsetX;
	float offsetY;

};

