#pragma once

#include "Player.h"

#include <vector>

/// <summary>
/// Manages the order of player turns. Handles the allocation and management of action points each turn.
/// </summary>
class TurnManager
{
public:

	unsigned int numberOfPlayers;
	std::vector<Player*> players;
};