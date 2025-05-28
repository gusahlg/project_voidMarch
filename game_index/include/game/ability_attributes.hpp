#include <raylib.h>
#include <vector>
#include <iostream>
#pragma once

struct Level{
    std::vector<std::string> rows;
    Vector2 playerPos;
};
extern bool moving;
extern int playerID;
extern int loadID;
extern bool isWall(int cx, int cy, Level& lvl);
void rollAbility(Level& lvl);
void abilityInputHandler(Level& lvl);