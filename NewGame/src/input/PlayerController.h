#pragma once

#include "../util/Time.h"
#include "../render/RenderingSystem.h"

enum PlayerMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	JUMP,
	ATTACK,
	UP,
	DOWN
};

class PlayerController
{
public:

	float currentSpeed;
	float currentTurnSpeed;
	float currentFlySpeed;
	bool freeCamera;

	PlayerController();
	void Reset();
	void Update(RenderingSystem* renderingSystem);
	void HandleInput(PlayerMovement input);
	void HandleMouseInput(float xOffset, float yOffset);

private:

	Time* time;

	const float RUN_SPEED = 200;
	const float TURN_SPEED = 200;
	const float FLY_SPEED = 200;
};