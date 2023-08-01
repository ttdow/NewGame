#include "GameState.h"

#include "../StateManager.h"
#include "../GameManager.h"

GameState::GameState(GameManager* gameManager, StateManager* stateManager)
{
	this->name = "default name";
	this->gameManager = gameManager;
	this->stateManager = stateManager;
}

void GameState::Enter()
{
	std::cout << "Entering: " << this->stateManager->currentState->name << std::endl;
}

void GameState::Exit()
{
	std::cout << "Exiting: " << this->stateManager->currentState->name << std::endl;
}

void GameState::HandleInput()
{

}

void GameState::LogicUpdate()
{

}

void GameState::PhysicsUpdate()
{

}