#include <raylib.h>
#include <iostream>
#include "../include/game/player_stats.hpp"
#include <cstdint>
#include <vector>
#include <cmath>
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
race currentRace;