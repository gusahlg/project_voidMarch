#pragma once
constexpr int TILE_SIZE = 16;
constexpr float PLAYER_WALK_SPEED = 5.f;

// Player position info:
extern int pSizeW;
extern int pSizeH;
extern float pPixX;
extern float pPixY;
extern Rectangle playerRect;

// Scalesystem:
extern ScaleSystem scaleSys;

// Player data:
extern Player gPlayer;