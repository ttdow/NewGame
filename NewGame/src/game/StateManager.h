#pragma once

#include "states/GameState.h"

/// <summary>
/// Class responsible for managing finite state machines.
/// </summary>
class StateManager
{
public:

	GameState* currentState;

	void Initialize(GameState* startingState);
	void ChangeState(GameState* newState);
};