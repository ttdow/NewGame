#pragma once

#include "../GameManager.h"

#include <iostream>
#include <string>

class StateManager;

class GameState
{
public:
	
	std::string name;
	GameManager* gameManager;
	StateManager* stateManager;

	GameState(GameManager* gameManager, StateManager* stateManager);

	virtual void Enter();
	virtual void Exit();
	virtual void HandleInput();
	virtual void LogicUpdate();
	virtual void PhysicsUpdate();
};