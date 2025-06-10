#include <raylib.h>
#include <iostream>
#include "../include/game/loadGame.hpp"
#include "../include/game/ability_attributes.hpp"
#include <vector>
#include <cmath>
#include <raymath.h>
#include <algorithm>
// Here things that can happen in game is defined. 
struct void_crawler_roll{
    Texture2D pos;
};
void_crawler_roll Up;
void_crawler_roll Down;
void_crawler_roll Left;
void_crawler_roll Right;
void loadRollTex(){
    Up.pos = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll4.png");
    Down.pos = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll1.png");
    Left.pos = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll2.png");
    Right.pos = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll1.png");
}
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
    switch(loadID){
        case(1):
            if(y <= Oy - rollDistance){
                rolling = false;
            }
            yofset -= 1.0f;
            playerTex = Up.pos;
            break;
        case(2):
            if(y >= Oy + rollDistance){
                rolling = false;
            }
            yofset += 1.0f;
            playerTex = Down.pos;
            break;
        case(3):
            if(x <= Ox - rollDistance){
                rolling = false;
            }
            xofset -= 1.0f;
            playerTex = Left.pos;
            break;
        default:
            if(x >= Ox + rollDistance){
                rolling = false;
            }
            xofset += 1.0f;
            playerTex = Right.pos;
            break;
    }
    float newX = x + xofset * ROLL_SPEED * dt;
    float newY = y + yofset * ROLL_SPEED * dt;
    if(!collisionRect(newX, newY, PLAYERWIDTH, PLAYERHEIGHT, lvl)){
        lvl.playerPos.x = newX;
        lvl.playerPos.y = newY;
    }
    else{
        rolling = false;
    }
}

//Weapons and their abilities are defined bellow.
struct projectile{
    Vector2 pos;
    Vector2 vel;
    float w, h;
    bool alive;
    projectile(Vector2 p, Vector2 v, float w_, float h_)
        : pos(p), vel(v), w(w_), h(h_), alive(true) {}
};
std::vector<projectile> bullets;
const float projW = 20;
const float projH = 15;
const float projSpeed = 100.0f;
void spawnProjectile(Vector2 startpos, Vector2 dir, float w, float h, float speed){
    projActive = true;
    bullets.emplace_back(
        startpos,
        Vector2Scale(dir, speed),
        w, h
    );
}
void updateProjectiles(Level& lvl, float dt){
    for(auto& b : bullets){
        if(!b.alive) continue;
        b.pos.x += b.vel.x * dt;
        b.pos.y += b.vel.y * dt;
        if(collisionRect(b.pos.x, b.pos.y, b.w, b.h, lvl)){
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
        if(!b.alive) continue;
        DrawRectangle((int)b.pos.x, (int)b.pos.y, (int)b.w, (int)b.h, RED);
    }
}
void updateRangedAttack(Vector2 pos, Vector2 dir, float dt, Level& lvl){
    bullets.reserve(512);
    /* Gonna add in stuff for drawing in the actual weapon as well. 
    DrawTexturePro()*/
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        spawnProjectile(pos, dir, projW, projH, projSpeed);
    }
    updateProjectiles(lvl, dt);
    drawProjectiles();
}