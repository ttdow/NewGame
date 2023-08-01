#include "StateManager.h"

void StateManager::Initialize(GameState* startingState)
{
	// Set the initial state and enter it.
	this->currentState = startingState;
	startingState->Enter();
}

void StateManager::ChangeState(GameState* newState)
{
	// Leave current state.
	this->currentState->Exit();

	// Enter the new state.
	this->currentState = newState;
	this->currentState->Enter();
}