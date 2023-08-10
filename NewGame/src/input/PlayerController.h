#pragma once

#include "../util/Time.h"
#include "../render/RenderingSystem.h"

enum PlayerMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	JUMP
};

class PlayerController
{
public:

	float currentSpeed;
	float currentTurnSpeed;
	bool freeCamera;

	PlayerController();
	void Reset();
	void Update(RenderingSystem* renderingSystem);
	void HandleInput(PlayerMovement input);
	void HandleMouseInput(float xOffset, float yOffset);

private:

	Time* time;

	const float RUN_SPEED = 50;
	const float TURN_SPEED = 20;
};