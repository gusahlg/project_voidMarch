#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>

struct Level{
    std::vector<std::string> rows;
    Vector2 playerPos;
};
extern bool moving;
extern int playerID;
extern int loadID;

extern Texture2D playerTex;
extern bool isWall(int cx, int cy, Level& lvl);
void abilityInputHandler(Level& lvl);