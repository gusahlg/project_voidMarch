#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
bool CircleSectorColl(float radius, Vector2 midpos, Rectangle enemy, Vector2 dir, float ArcAngle);
extern Vector2 Mcenter; extern float Mradius; extern float Mstart; extern float Mend; extern float MarcSize; extern Vector2 Mdir;
void entityCollisionCheck();
void meleeAttack();
void spawnLogic(Vector2 pos, int HP, int ID);
void enemyLogic(float dt, Level& lvl, Vector2 playerCenter);