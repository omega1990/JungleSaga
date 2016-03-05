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
	void SetGemColor(King::Engine::Texture color);

	float mouseMoveStartX;
	float mouseMoveStartY;
	
	direction moveDirection;

	bool Selected;

	void SetOffsetX(float passedOffsetX);
	void SetOffsetY(float passedOffsetY);
	void SetOffset(float passedOffsetX, float passedOffsetY);
	void SetGravityOffsetY(float passedOffsetY);

	float GetOffset(direction dir, float mouseX, float mouseY);
	float GetOffsetX() { return offsetX; };
	float GetOffsetY() { return offsetY; };

	void SetToBeDestroyed(bool passedDestroyed);
	bool GetToBeDestroyed();

	void ResetOffset();

	void SetGravity(bool passedGravity) { gravity = passedGravity; };
	bool GetGravity() { return gravity; };

private:
	King::Engine::Texture gemColor;
	float offsetX;
	float offsetY;
	bool toBeDestroyed;

	bool gravity;
};

