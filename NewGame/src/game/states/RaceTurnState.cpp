#include "RaceTurnState.h"

RaceTurnState::RaceTurnState(GameManager* gameManager, StateManager* stateManager) : GameState(gameManager, stateManager)
{
	this->name = "default name";
	this->gameManager = gameManager;
	this->stateManager = stateManager;
}

void RaceTurnState::Enter()
{
	std::cout << "Here" << std::endl;
}

void RaceTurnState::Exit()
{
	std::cout << "Goodbye" << std::endl;
}