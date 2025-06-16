#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
struct Level{
    std::vector<std::string> rows;
    Vector2 playerPos;
};
struct projectile {
    Vector2 pos;
    Vector2 vel;
    float   w, h;
    bool    alive;
    projectile(Vector2 p, Vector2 v, float w_, float h_)
      : pos(p), vel(v), w(w_), h(h_), alive(true) {}
};
extern std::vector<projectile> bullets;
extern float scale;
extern bool rolling;
extern bool moving;
extern int playerID;
extern int loadID;
extern float Ox;
extern float Oy;
extern bool attacking;
extern float PLAYERWIDTH;
extern float PLAYERHEIGHT;
void spawnProjectile(Vector2 startpos, Vector2 dir, float w, float h, float speed);
void loadRollTex();
void updateMeleeAttack(Vector2 pos, Vector2 dir, float ARCSIZE, float radius, Level& lvl);
void updateRoll(Level& lvl, float dt);
extern Texture2D playerTex;
bool collisionRect(float cx, float cy, float cw, float ch, Level& lvl);
void updateRangedAttack(Vector2 pos, Vector2 dir, float projW, float projH, float projSpeed, float dt, Level& lvl);
void spawnProjectile(Vector2 startpos, Vector2 dir, float w, float h, float speed);
extern bool projActive;
