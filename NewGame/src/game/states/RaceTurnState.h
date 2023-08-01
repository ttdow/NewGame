#pragma once

#include "GameState.h"

#include "../GameManager.h"
#include "../StateManager.h"

class RaceTurnState : public GameState
{
public:

	RaceTurnState(GameManager* gameManager, StateManager* stateManager);

	void Enter() override;
	void Exit() override;
};