#include <raylib.h>
#include <iostream>
#include "../include/game/loadGame.hpp"
#include "../include/game/ability_attributes.hpp"
#include <vector>
#include <cmath>
// Here things that can happen in game is defined. 
void rollAbility(Level& lvl){
    float x = lvl.playerPos.x;
    float y = lvl.playerPos.y;
    const float animateDelay = 0.01;
    static float timer = 0.0f;
    timer -= GetFrameTime();
    if(timer >= 0.0f){
        return;
    }
    switch(loadID){
        case(1):
            y -= 0.1;
            break;
        case(2):
            y += 0.1;
            break;
        case(3):
            x -= 0.1;
            break;
        default:
            x += 0.1;
            break;
    }
    int rx = (int)std::floorf(x);
    int ry = (int)std::floorf(y);
    if(!isWall(rx, ry, lvl)){
        lvl.playerPos.x = x;
        lvl.playerPos.y = y;
    }
    timer = animateDelay;     
}
void abilityInputHandler(Level& lvl){
    if(IsKeyPressed(KEY_Q)){
        rollAbility(lvl);
    }
}