#include <raylib.h>
#include <iostream>
#include "../include/game/loadGame.hpp"
#include "../include/game/ability_attributes.hpp"
#include <vector>
#include <cmath>
int LAYER_FRAMES = 4;

int urrentFrame = 0;

// Here things that can happen in game is defined. 
struct void_crawler_roll{
    Texture2D pos;
};
void_crawler_roll Up;
void_crawler_roll Down;
void_crawler_roll Left;
void_crawler_roll Right;
void loadRollTex(){
    Up.pos = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll4");
    Down.pos = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll1");
    Left.pos = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll2");
    Right.pos = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll3");
}
bool rolling;
float Ox;
float Oy;
void updateRoll(Level& lvl){
    int rollDistance = 3;
    rolling = true;
    float x = lvl.playerPos.x;
    float y = lvl.playerPos.y;
    switch(loadID){
        case(1):
            if(y <= Oy - rollDistance){
                rolling = false;
            }
            y -= 0.2f;
            break;
        case(2):
            if(y >= Oy + rollDistance){
                rolling = false;
            }
            y += 0.2;
            break;
        case(3):
            if(x <= Ox - rollDistance){
                rolling = false;
            }
            x -= 0.2;
            break;
        default:
            if(x >= Ox + rollDistance){
                rolling = false;
            }
            x += 0.2;
            break;
    }
    int rx = (int)std::floorf(x);
    int ry = (int)std::floorf(y);
    if(!isWall(rx, ry, lvl)){
        lvl.playerPos.x = x;
        lvl.playerPos.y = y;
    }
}
void abilityInputHandler(Level& lvl){
    if(IsKeyPressed(KEY_Q) || rolling){
        updateRoll(lvl);
    }
    else if(!rolling){
        Ox = lvl.playerPos.x;
        Oy = lvl.playerPos.y;
    }
}