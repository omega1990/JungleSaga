#include "Gem.h"

Gem::Gem(King::Engine::Texture passedGemColor)
	: gemColor(passedGemColor)
	, offsetX(0.0f)
	, offsetY(0.0f)
	, moveDirection(Gem::STATIONARY)
	, gravity(false)
	, toBeDestroyed(false)
	, Selected(false)
{
}

Gem::~Gem()
{
}

King::Engine::Texture Gem::GetGemColor()
{
	return gemColor;
}

float Gem::GetOffset(direction dir, float mouseX, float mouseY)
{
	switch (dir)
	{
	case HORIZONTAL_LEFT:
		return mouseMoveStartX - mouseX;
	case HORIZONTAL_RIGHT:
		return mouseX - mouseMoveStartX;
	case VERTICAL_UP:
		return mouseY - mouseMoveStartY;
	case VERTICAL_DOWN:
		return mouseMoveStartY - mouseY;
	default:
		return 0.0f;
	}
}

void Gem::SetOffset(float passedOffsetX, float passedOffsetY)
{
	if (passedOffsetX > MAX_GEM_OFFSET)
	{
		offsetX = MAX_GEM_OFFSET;
	}
	else if (passedOffsetX < -MAX_GEM_OFFSET)
	{
		offsetX = -MAX_GEM_OFFSET;
	}
	else
		offsetX = passedOffsetX;

	if (passedOffsetY > MAX_GEM_OFFSET)
	{
		offsetY = MAX_GEM_OFFSET;
	}
	else if (passedOffsetY < -MAX_GEM_OFFSET)
	{
		offsetY = -MAX_GEM_OFFSET;
	}
	else
		offsetY = passedOffsetY;
}

void Gem::SetOffsetX(float passedOffsetX)
{
	if (passedOffsetX > MAX_GEM_OFFSET)
	{
		offsetX = MAX_GEM_OFFSET;
	}
	else if (passedOffsetX < -MAX_GEM_OFFSET)
	{
		offsetX = -MAX_GEM_OFFSET;
	}
	else
		offsetX = passedOffsetX;
}

void Gem::SetOffsetY(float passedOffsetY)
{
	if (passedOffsetY > MAX_GEM_OFFSET)
	{
		offsetY = MAX_GEM_OFFSET;
	}
	else if (passedOffsetY < -MAX_GEM_OFFSET)
	{
		offsetY = -MAX_GEM_OFFSET;
	}
	else
		offsetY = passedOffsetY;
}

void Gem::SetGravityOffsetY(float passedOffsetY)
{
	offsetY = passedOffsetY;
}

void Gem::ResetOffset()
{
	offsetX = 0.0f;
	offsetY = 0.0f;
}

void Gem::SetGemColor(King::Engine::Texture color)
{
	gemColor = color;
}


void Gem::SetToBeDestroyed(bool passedDestroyed)
{
	toBeDestroyed = passedDestroyed;
}

bool Gem::GetToBeDestroyed()
{
	return toBeDestroyed;
}

