#include <raylib.h>
#include <iostream>
#include "../include/game/loadGame.hpp"
#include "../include/game/ability_attributes.hpp"
#include <vector>
#include <cmath>
#include <raymath.h>
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
};
std::vector<projectile> bullets;
float projX = 0;
float projY = 0;
float dirX, dirY;
bool projActive = false;
bool targetHit = false;
float projectileSpeed = 300.0f;
float projW = 20;
float projH = 15;
float projSpeed = 300.0f;
int projCount = 1;
void spawnProjectile(Vector2 startpos, Vector2 dir, float w, float h, float speed){
    bullets.emplace_back(projectile{
        startpos,
        Vector2Scale(dir, speed),
        w, h,
        true
    });
}
void updateRangedAttack(float x, float y, float dt, Level& lvl){
    /* Gonna add in stuff for drawing in the actual weapon as well. 
    DrawTexturePro()*/
    for(int i = 0; i < projCount; ++i){
        spawnProjectile(x, y, projW, projH, projSpeed);
    }
    if(!targetHit){
        projCount += 1;
    }
    else{
        if(projCount < 1){
            projCount -= 1;
        }
    }
}