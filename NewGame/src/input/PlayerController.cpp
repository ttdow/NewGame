#include "PlayerController.h"

PlayerController::PlayerController()
{
	this->Reset();

	this->time = Time::GetInstance();

	this->freeCamera = false;
}

void PlayerController::Reset()
{
	this->currentSpeed = 0.0f;
	this->currentTurnSpeed = 0.0f;
}

void PlayerController::Update(RenderingSystem* renderingSystem)
{
	glm::vec3 rotate = glm::vec3(0.0f, this->currentTurnSpeed, 0.0f) * this->time->GetDeltaTime();
	glm::vec3 orientation = *renderingSystem->goblin->GetTransform()->rotation;
	orientation = orientation + rotate;

	float distance = this->currentSpeed * this->time->GetDeltaTime();

	float dx = distance * glm::sin(orientation.y);
	float dz = distance * glm::cos(orientation.y);

	glm::vec3 move = glm::vec3(dx, 0.0f, dz);
	glm::vec3 position = *renderingSystem->goblin->GetTransform()->position;
	position += move;

	renderingSystem->goblin->GetTransform()->Update(position, orientation);
}

void PlayerController::HandleInput(PlayerMovement input)
{
	switch (input)
	{
		case FORWARD:
			this->currentSpeed = RUN_SPEED;
			break;
		case BACKWARD:
			this->currentSpeed = -RUN_SPEED;
			break;
		case LEFT:
			this->currentTurnSpeed = TURN_SPEED;
			break;
		case RIGHT:
			this->currentTurnSpeed = -TURN_SPEED;
			break;
		case JUMP:
			// TODO
			break;
		case ATTACK:
			// TODO
			break;
	}
}

void PlayerController::HandleMouseInput(float xOffset, float yOffset)
{
	if (this->freeCamera)
	{
		this->currentTurnSpeed = -xOffset;
	}
}