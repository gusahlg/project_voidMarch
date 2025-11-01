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
#include "../include/global/scale_utils.hpp"
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