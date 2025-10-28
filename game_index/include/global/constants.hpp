#pragma once
#include <raylib.h>
#include "../data/stats/player.hpp"
#include "../data/stats/world.hpp"
constexpr int TILE_SIZE = 16;
constexpr float PLAYER_WALK_SPEED = 5.f;

// Player position info:
extern int pSizeW;
extern int pSizeH;
extern float pPixX;
extern float pPixY;
extern Rectangle playerRect;


// Player data:
inline Player gPlayer{};
inline World gWorld{};