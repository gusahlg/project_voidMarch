#include <raylib.h>
#include <iostream>
#include "../include/select/Sbuttons.hpp"
#include "../include/game/loadGame.hpp"
#include "../include/game/player_stats.hpp"
#include "../include/game/ability_attributes.hpp"
#include "../include/game/enemy_data.hpp"
#include <vector>
#include <fstream>
#include <cmath>
#include <raymath.h>
#include <cstdint>
// This is for handling different player states, stats (two similar words after each other, don't fall asleep my fellow 'stay up late gamedev') such as health and attributes such as 'intelligence'.
enum class Stat : uint8_t{
    HP, // Hitpoints 
    MP, // Mana or something
    STR,// Strength
    AGI,// Agility
    INT,// Intelligence
    _COUNT
};
struct Stats{
    static constexpr size_t COUNT = static_cast<size_t>(Stat::_COUNT);
    

};

