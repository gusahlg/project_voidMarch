#include <raylib.h>
#include <iostream>
#include "../include/game/player_stats.hpp"
#include <cstdint>
#include <vector>
#include <cmath>
// Gives easy access to all standard ui features:
#include "../include/standardUI/manager.hpp"
bool raceSPACELIZARD = false;
bool raceVOIDCRAWLER = false;
bool raceMECHA_SAPIEN = false;
bool raceHUMAN = true;

bool profNECROMANCER = true;
bool profCRAFTSMAN = false;
bool profWIZARD = false;
bool profDUALIST = false;
enum class prof : std::uint8_t{Necromancer, Craftsman, Wizard, Dualist};
prof currentProf;
enum class race : std::uint8_t{Spacelizard, Voidcrawler, Mecha_sapien, Human};
race currentRac; // Can't be named 'currentRace' since that is the same as another enum in game.cpp, will fix at some point.

void loadSelectScreen(){
    // Update and initialise buttons here please.
}