#include "Gem.h"

Gem::Gem(King::Engine::Texture passedGemColor)
	: gemColor(passedGemColor)
	, offsetX(0.0f)
	, offsetY(0.0f)
	, gravity(false)
	, toBeDestroyed(false)
{
}

Gem::~Gem()
{
}

/// <summary> Gets the color of the gem </summary>
King::Engine::Texture Gem::GetGemColor()
{
	return gemColor;
}

/// <summary> Gets gems offset depending of the direction in which it is in offset </summary>
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

/// <summary> Set gem offset to specific value </summary>
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

/// <summary> Set gem offset X to specific value </summary>
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


/// <summary> Set gem offset Y to specific value </summary>
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

/// <summary> Sets how much the gem will be launched up so that it can be pulled by gravity pull </summary>
void Gem::SetGravityOffsetY(float passedOffsetY)
{
	offsetY = passedOffsetY;
}


/// <summary> Resets offsets to default values </summary>
void Gem::ResetOffset()
{
	offsetX = 0.0f;
	offsetY = 0.0f;
}

/// <summary> Sets gem color </summary>
void Gem::SetGemColor(King::Engine::Texture color)
{
	gemColor = color;
}

/// <summary> Sets toBeDestroyed </summary>
void Gem::SetToBeDestroyed(bool passedDestroyed)
{
	toBeDestroyed = passedDestroyed;
}

/// <summary> Gets toBeDestroyed </summary>
bool Gem::GetToBeDestroyed()
{
	return toBeDestroyed;
}

/// <summary> Moves gem to right </summary>
void Gem::MoveRight()
{
	SetOffsetX(GetOffsetX() + gemMoveSpeed);
}

/// <summary> Moves gem to left </summary>
void Gem::MoveLeft()
{
	SetOffsetX(GetOffsetX() - gemMoveSpeed);
}

/// <summary> Moves gem down </summary>
void Gem::MoveDown()
{
	SetOffsetY(GetOffsetY() + gemMoveSpeed);
}

/// <summary> Moves gem up </summary>
void Gem::MoveUp()
{
	SetOffsetY(GetOffsetY() - gemMoveSpeed);
}

/// <summary> Saves mouse position from when gem was selected </summary>
void Gem::SetMouseMoveStart(float x, float y)
{
	mouseMoveStartX = x;
	mouseMoveStartY = y;
}

/// <summary> Gets mouse position from when the game was selected </summary>
std::pair<float, float> Gem::getMouseMoveStart()
{
	return std::make_pair(mouseMoveStartX, mouseMoveStartY);
}

