#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
void entityCollisionCheck();
void spawnLogic(Vector2 pos, int HP, int ID);
void enemyLogic(float dt, Level& lvl, Vector2 playerCenter);