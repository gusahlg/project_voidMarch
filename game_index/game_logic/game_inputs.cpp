#include <raylib.h>
#include <iostream>
#include "../include/game/loadGame.hpp"
#include "../include/game/ability_attributes.hpp"
#include <vector>
#include <cmath>
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
float projectileStepX;
float projectileStepY;
bool targetHit = false;
float projectileSpeed = 10;
float projectileW = 50;
float projectileH = 50;
bool pMove = false;
void spawnProjectile(float x, float y, float w, float h, float dt, Level lvl){
    if(collisionRect(x + projectileStepX * projectileSpeed * dt, y + projectileStepY * projectileSpeed * dt, w, h, lvl)){
        targetHit = true;
        projectileStepX = 0.0f;
        projectileStepY = 0.0f;
        pMove = false;
    }
    else{
        projectileStepX += 1;
        DrawRectangle(x + projectileStepX * projectileSpeed * dt, y + projectileStepY * projectileSpeed * dt, w, h, RED);
    }
}
void updateRangedAttack(float x, float y, float dt, Level lvl){
    /* Gonna add in stuff for drawing in the actual weapon as well.
    DrawTexturePro()*/
    pMove = true;
    spawnProjectile(x, y, projectileW, projectileH, dt, lvl);
}