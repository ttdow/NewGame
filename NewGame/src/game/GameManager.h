#pragma once

#include "Player.h"

#include <vector>

/// <summary>
/// Class responsible for managing game data.
/// </summary>
class GameManager
{
public:

	unsigned int numberOfPlayers;
	std::vector<Player> players;

	GameManager();
};