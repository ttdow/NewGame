#pragma once

#include "StateManager.h"

/// <summary>
/// Manages the overall flow of a race, including the start, progress, and finish of each race.
///	Handles race - specific events, scoring, and result calculation.
/// </summary>
class RaceManager
{
public:

	StateManager* raceStateManager;

};