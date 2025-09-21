#include <raylib.h>
#include <iostream>
#include "../include/game/loadGame.hpp"
#include "../include/game/ability_attributes.hpp"
#include "../include/game/enemy_data.hpp"
#include <vector>
#include <cmath>
#include <raymath.h>
#include <algorithm>
// Essential systems used for scaling and communicating constants.
#include "../include/global/constants.hpp"
#include "../include/global/scale_system.hpp"
bool rolling;
float Ox;
float Oy;
void updateRoll(Level& lvl, float dt){
    const float ROLL_SPEED = 11.0f;
    const int rollDistance = 5;
    rolling = true;
    float x = lvl.playerPos.x;
    float y = lvl.playerPos.y;
    float xofset = 0.0f;
    float yofset = 0.0f;
    switch(currentDir){
        case Up:
            if (y <= Oy - rollDistance) {
                rolling = false;
            }
            yofset -= 1.0f;
            break;
        case Down:
            if (y >= Oy + rollDistance) {
                rolling = false;
            }
            yofset += 1.0f;
            break;
        case Left:
            if (x <= Ox - rollDistance) {
                rolling = false;
            }
            xofset -= 1.0f;
            break;
        case UpLeft:
            if (sqrtf((x - Ox)*(x - Ox) + (y - Oy)*(y - Oy)) >= rollDistance) {
                rolling = false;
            }
            xofset -= 1.0f;
            yofset -= 1.0f;
            break;
        case UpRight:
            if (sqrtf((x - Ox)*(x - Ox) + (y - Oy)*(y - Oy)) >= rollDistance) {
                rolling = false;
            }
            xofset += 1.0f;
            yofset -= 1.0f;
            break;
        case DownLeft:
            if (sqrtf((x - Ox)*(x - Ox) + (y - Oy)*(y - Oy)) >= rollDistance) {
                rolling = false;
            }
            xofset -= 1.0f;
            yofset += 1.0f;
            break;
        case DownRight:
            if (sqrtf((x - Ox)*(x - Ox) + (y - Oy)*(y - Oy)) >= rollDistance) {
                rolling = false;
            }
            xofset += 1.0f;
            yofset += 1.0f;
            break;
        default: // Right
            if(x >= Ox + rollDistance) {
                rolling = false;
            }
            xofset += 1.0f;
            break;
    }
    float newX = x + xofset * ROLL_SPEED * dt;
    float newY = y + yofset * ROLL_SPEED * dt;
    if (!collisionRect(newX, newY, PLAYERWIDTH, PLAYERHEIGHT, lvl)) {
        lvl.playerPos.x = newX;
        lvl.playerPos.y = newY;
    }
    else {
        rolling = false;
    }
}
//Weapons and their abilities are defined bellow.
std::vector<projectile> bullets;
void spawnProjectile(Vector2 startpos, Vector2 dir, float w, float h, float speed, bool enemyOwner){
    projActive = true;
    bullets.emplace_back(
        startpos,
        Vector2Scale(dir, speed),
        w, h, enemyOwner
    );
}
void updateProjectiles(Level& lvl, float dt){
    for(auto& b : bullets){
        if(!b.alive) continue;

        b.pos.x += b.vel.x * dt;
        b.pos.y += b.vel.y * dt;
        auto& si = scaleSys.info();
        float gx = toTiles(b.pos.x, si);
        float gy = toTiles(b.pos.y, si);
        float gw = toTiles((b.w*si.scale), si);
        float gh = toTiles((b.h*si.scale), si);
        if(collisionRect(gx, gy, gw, gh, lvl)){
            b.alive = false;
        }
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const projectile& p){return !p.alive;}), bullets.end());
    if(bullets.empty()){
        projActive = false;
    }
}
void drawProjectiles(){
    for(const auto& b : bullets){
        auto& si = scaleSys.info();
        float gw = toTiles((b.w*si.scale), si);
        float gh = toTiles((b.h*si.scale), si);
        DrawRectangle((int)b.pos.x, (int)b.pos.y, (int)gw, (int)gh, RED);
    }
}
void updateRangedAttack(Vector2 pos, Vector2 dir, float projW, float projH, float projSpeed, float dt, Level& lvl){
    updateProjectiles(lvl, dt);
    drawProjectiles();
}
// clamp = forces something to stay within a range min - max.
bool CircleSectorColl(float radius, Vector2 midpos, Rectangle enemy, Vector2 dir, float ArcAngle){
    Vector2 closest;
    closest.x = std::clamp(midpos.x, enemy.x, enemy.x + enemy.width);
    closest.y = std::clamp(midpos.y, enemy.y, enemy.y + enemy.height);
    float dx = closest.x - midpos.x; float dy = closest.y - midpos.y;
    float dist2 = dx * dx + dy * dy;
    Vector2 enemyCenter = {enemy.x + enemy.width / 2.0f, enemy.y + enemy.height / 2.0f};
    Vector2 v = Vector2Subtract(enemyCenter, midpos);
    float len = sqrtf(v.x * v.x + v.y * v.y);
    if(len == 0.0f) return true;
    float dot = dir.x * v.x + dir.y * v.y;
    float cosTheta = dot / len;
    float halfArc = (ArcAngle * DEG2RAD) * 0.5f;
    float cosEdge = cosf(halfArc);                 
    float halfDiag = 0.5f * sqrtf(enemy.width * enemy.width + enemy.height * enemy.height);
    if(halfDiag < len){                     
        float beta = asinf( halfDiag / len );
        cosEdge = cosf(halfArc + beta);
    }
    float s = sinf(halfArc);
    float c = cosf(halfArc);
    Vector2 edgeL = {dir.x * c - dir.y * s,  dir.x * s + dir.y * c};
    Vector2 edgeR = {dir.x * c + dir.y * s,  dir.y * c - dir.x * s};
    Vector2 A = {midpos.x + edgeL.x * radius, midpos.y + edgeL.y * radius};
    Vector2 B = {midpos.x + edgeR.x * radius, midpos.y + edgeR.y * radius};
    Vector2 TL = {enemy.x, enemy.y};
    Vector2 TR = {enemy.x + enemy.width,   enemy.y};
    Vector2 BR = {TR.x, enemy.y + enemy.height};
    Vector2 BL = {TL.x, BR.y};
    if(!(dist2 <= radius * radius)) return false;
    if(cosTheta < cosEdge) return false;
    if((dist2 <= radius*radius) && (cosTheta >= cosEdge)) return true;
    if(CheckCollisionLines(midpos, A, TL, TR, nullptr) || CheckCollisionLines(midpos, A, TR, BR, nullptr) || CheckCollisionLines(midpos, A, BR, BL, nullptr) || CheckCollisionLines(midpos, A, BL, TL, nullptr) || CheckCollisionLines(midpos, B, TL, TR, nullptr) || CheckCollisionLines(midpos, B, TR, BR, nullptr) || CheckCollisionLines(midpos, B, BR, BL, nullptr) || CheckCollisionLines(midpos, B, BL, TL, nullptr)) return true;
    else return false;
}
inline float Vec2AngleDeg(Vector2 v){
    return atan2f(v.y, v.x) * RAD2DEG;
}
inline float Norm360(float a){
    a = fmodf(a + 360.0f, 360.0f);
    return a;
}
Vector2 Mcenter; float Mradius; float Mstart; float Mend; float MarcSize; Vector2 Mdir;
void defineDamageArea(Vector2 centerpos, float radius, Vector2 dir, float ARCSIZE){       
    float mid = Vec2AngleDeg(dir);            
    float start = Norm360(mid - ARCSIZE);       
    float end = Norm360(mid + ARCSIZE);
    //Passing on variables.
    Mcenter = centerpos; Mradius = radius; Mstart = start; Mend = end; MarcSize = ARCSIZE; Mdir = dir;
}
Rectangle Esrc = {0, 0, 32, 32};
void updateMeleeAttack(Vector2 pos, Vector2 dir, float ARCSIZE, float radius, Level& lvl, Rectangle dest, Vector2 origin, float rotation){
    /*Add in attack animation (swing a sword or something)*/
}