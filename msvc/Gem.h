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

	void SetOffsetX(float passedOffsetX);
	void SetOffsetY(float passedOffsetY);
	void SetOffset(float passedOffsetX, float passedOffsetY);
	void SetGravityOffsetY(float passedOffsetY);

	float GetOffset(direction dir, float mouseX, float mouseY);
	float GetOffsetX() { return offsetX; };
	float GetOffsetY() { return offsetY; };

	void SetToBeDestroyed(bool passedDestroyed);
	bool GetToBeDestroyed();
	
	void MoveRight();
	void MoveLeft();
	void MoveDown();
	void MoveUp();


	void SetMouseMoveStart(float x, float y);
	std::pair<float,float> getMouseMoveStart();

	void ResetOffset();


	const float gemMoveSpeed = 3.0f;

private:
	King::Engine::Texture gemColor;
	float offsetX;
	float offsetY;
	bool toBeDestroyed;

	float mouseMoveStartX;
	float mouseMoveStartY;

	bool gravity;
};

