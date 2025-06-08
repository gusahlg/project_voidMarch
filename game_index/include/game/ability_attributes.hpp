#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>

struct Level{
    std::vector<std::string> rows;
    Vector2 playerPos;
};
extern bool rolling;
extern bool moving;
extern int playerID;
extern int loadID;
extern float Ox;
extern float Oy;
extern float PLAYERWIDTH = 0.5;
extern float PLAYERHEIGHT = 0.5;
void loadRollTex();
void updateRoll(Level& lvl, float dt);
extern Texture2D playerTex;
bool collisionRect(float cx, float cy, float cw, float ch, Level& lvl);