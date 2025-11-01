#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include "ability_attributes.hpp"

class Level;  
void spawnLogic(Vector2 pos, int HP, int ID);
void enemyLogic(float dt, Level& lvl, Vector2 playerCenter);
extern std::vector<enemy> enemies;