#include "GameManager.h"

GameManager::GameManager()
{
	this->numberOfPlayers = 1;
	this->players = std::vector<Player>();
	this->players.emplace_back(1);
}