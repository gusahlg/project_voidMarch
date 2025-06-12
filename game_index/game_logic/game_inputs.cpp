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
std::vector<projectile> bullets;
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
        float gx = b.pos.x / (16 * scale); 
        float gy = b.pos.y / (16 * scale); 
        float gw = b.w / (16 * scale); 
        float gh = b.h / (16 * scale);
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
        if(!b.alive) continue;
        DrawRectangle((int)b.pos.x, (int)b.pos.y, (int)b.w, (int)b.h, RED);
    }
}
void updateRangedAttack(Vector2 pos, Vector2 dir, float projW, float projH, float projSpeed, float dt, Level& lvl){
    /* Gonna add in stuff for drawing in the actual weapon as well. 
    DrawTexturePro()*/
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        spawnProjectile(pos, dir, projW, projH, projSpeed);
    }
    updateProjectiles(lvl, dt);
    drawProjectiles();
}
struct damageZone{
    Vector2 lowerLeftpos;
    float lw;
    Vector2 higherLeftpos;
    float hw;
    damageZone(float lw, float hw, Vector2 lowerLeftpos, Vector2 higherLeftpos)
    : lw(lw), hw(hw), lowerLeftpos(lowerLeftpos), higherLeftpos(higherLeftpos) {};    
};
void damageZoneCalc(Vector2 angles, Vector2 armLength, Vector2 pos, float width){
    float xModifier1 = angles.x / 180.0f;
    float yModifier1 = 1.0f - xModifier1;
    float xModifier2 = angles.y / 180.0f;
    float yModifier2 = 1.0f - xModifier2;
    Vector2 higherLeftpos = {pos.x + armLength.x * xModifier1,
                            pos.y + armLength.x * yModifier1};
    Vector2 lowerLeftpos = {pos.x + width + armLength.y * xModifier2,
                            pos.y + armLength.y * yModifier2};
    float higherWidth = width + armLength.x * xModifier1 + armLength.y * xModifier2;
    damageZone(width, higherWidth, lowerLeftpos, higherLeftpos); 
}
void updateMeleeAttack(){
    /*Add in attack animation (swing a sword or something)*/
    // Everything within an area based one player pos and mouse direction gets damaged.

}