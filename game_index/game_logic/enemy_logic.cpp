#include <raylib.h>
#include <iostream>
#include "../include/game/ability_attributes.hpp"
#include "../include/game/enemy_data.hpp"
#include <vector>
#include <cmath>
#include <raymath.h>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstdint>
#include "../include/game/global_player.hpp" // Player stats, all in one place.
#include "../include/global/scale.hpp"
struct enemy{
    int frames;
    int currentFrame;
    float animDelay;
    float animtimer;
    Rectangle Hbox;
    int MAXHP;      // Maximum HP.
    int HP;         // Actual HP, mutable.
    float cooldown; // Attack cooldown duration
    float range;    // Detection range
    Texture2D tex;  // Enemy texture
    enum dir : std::uint8_t{Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight};
    dir currentDir = Up;
    enum state : std::uint8_t{Idle, Walking, Jumping};
    state currentState = Idle;
    enum class Type : std::uint8_t{generic, TurtleMaster, Bob}; //Expand for more variations
    Type kind = Type::generic;
    // Bellow depends on type.
    float speed{};
    void determineState(Vector2 playerPixCenter){
        Vector2 Emid = {Hbox.x + Hbox.width/2.0f, Hbox.y + Hbox.height/2.0f};
        bool inRangeX = std::fabs(Emid.x - playerPixCenter.x) <= range;
        bool inRangeY = std::fabs(Emid.y - playerPixCenter.y) <= range;
        float distX = Emid.x < playerPixCenter.x ? playerPixCenter.x - Emid.x : Emid.x - playerPixCenter.x;
        float distY = Emid.y < playerPixCenter.y ? playerPixCenter.y - Emid.y : Emid.y - playerPixCenter.y;
        float dx = std::fabs(playerPixCenter.x - Emid.x);
        float dy = std::fabs(playerPixCenter.y - Emid.y);
        // Add in randomness and such for good pathfinding, current system is temporary
        if(inRangeX && inRangeY){
            currentState = state::Walking;
        }
        else currentState = state::Idle;
        // Check if diagonals are faster, then left/right, up/down.
        if(dx >= 1.0f && dy >= 1.0f){ 
            bool right = Emid.x < playerPixCenter.x;
            bool down  = Emid.y < playerPixCenter.y;
            currentDir = right
                        ? (down ? DownRight : UpRight)
                        : (down ? DownLeft  : UpLeft);
        }
        else if(dx > dy){  
            currentDir = (Emid.x < playerPixCenter.x) ? Right : Left;
        }
        else if(dy > 0.0f){
            currentDir = (Emid.y < playerPixCenter.y) ? Down : Up;
        }
        else{
            currentState = Idle;
        }
    }
    // Determine if damage should be dealt to player.
    bool playerInRange(float range, Vector2 playerPixCenter){
        Vector2 Emid = {Hbox.x + Hbox.width/2.0f, Hbox.y + Hbox.height/2.0f};
        bool inRangeX = std::fabs(Emid.x - playerPixCenter.x) <= range;
        bool inRangeY = std::fabs(Emid.y - playerPixCenter.y) <= range;
        if(inRangeX && inRangeY) return true;
        else return false;
    }
    void draw(){
        DrawTexturePro(tex, {0,0,float(tex.width)/4,float(tex.height)}, Hbox, {0,0}, 0, WHITE);
    }
    enemy(Vector2 pos, int hp, Type t, float s)
    : Hbox{}, MAXHP(hp), HP(hp), kind(t)
{
    const size_t idx = static_cast<size_t>(kind);
    // Constant LUT 
    static constexpr float   speedLUT     [] = {90.f, 20.f, 50.f};
    static constexpr Vector2 sizeLUT      [] = {{10.f,10.f}, {20.f,20.f}, {25.f,25.f}};
    static constexpr float   delayLUT     [] = {30.f, 40.f, 50.f};
    static constexpr float   rangeLUT     [] = {50.f, 100.f, 25.f};
    static constexpr float   animDelayLUT [] = {10.f, 20.f, 30.f};
    static constexpr int     framesLUT    [] = {3, 4, 5};
    //Determined values from tables above
    speed     = speedLUT[idx] * s;
    cooldown  = delayLUT[idx] * s;
    range     = rangeLUT[idx] * s;
    animDelay = animDelayLUT[idx];
    frames    = framesLUT[idx];
    // Hitbox calc
    Vector2 sz = { sizeLUT[idx].x * s, sizeLUT[idx].y * s };
    Hbox = { pos.x, pos.y, sz.x, sz.y };

    // Texture determination:
    switch(idx){
        case 0:  break;
        case 1: tex = LoadTexture("assets/graphics/enemies/Turtlemaster.png"); break;
        case 2:  break;
    }
}
};
std::vector<enemy> enemies;
void spawnEnemy(Vector2 pos, int HP, enemy::Type t, float s){ //Add into level initialization and other stuff.
    enemies.emplace_back(pos, HP, t, s);
}
void spawnLogic(Vector2 pos, int HP, int ID, float scale){
    enemy::Type t;
    switch(ID){
        case(0): t = enemy::Type::generic; break;
        case(1): t = enemy::Type::TurtleMaster; break;
        case(2): t = enemy::Type::Bob; break;
        default: t = enemy::Type::generic; break;
    }
    spawnEnemy(pos, HP, t, scale);
}
// Idea: Add in types, speed and stuff in enemies struct
void updateEnemies(float dt, Level& lvl, Vector2 playerCenter){
    float tileSize = 16.0f * std::max(scale, 0.0001f);
    for(auto& e : enemies){
        e.determineState(playerCenter);
        Rectangle f = e.Hbox;
        switch(e.currentState){
            case(enemy::Walking):
            {
                float step = e.speed * dt;
                switch (e.currentDir){
                    case enemy::Up:        f.y -= step; break;
                    case enemy::Down:      f.y += step; break;
                    case enemy::Left:      f.x -= step; break;
                    case enemy::Right:     f.x += step; break;
                    case enemy::UpLeft:    f.x -= step * SQRT2; f.y -= step * SQRT2; break;
                    case enemy::UpRight:   f.x += step * SQRT2; f.y -= step * SQRT2; break;
                    case enemy::DownLeft:  f.x -= step * SQRT2; f.y += step * SQRT2; break;
                    case enemy::DownRight: f.x += step * SQRT2; f.y += step * SQRT2; break;
                }
                break;
            }
            case(enemy::Jumping):
                //Jump
                break;
            default:
                
                break;
        }
        float tileX = f.x / tileSize;
        float tileY = f.y / tileSize;
        float tileW = f.width  / tileSize;
        float tileH = f.height / tileSize;
        if(!collisionRect(tileX, tileY, tileW, tileH, lvl)){
            e.Hbox = f;
        }
    }
    enemies.erase(
        std::remove_if(
            enemies.begin(), enemies.end(), [](auto& p){
                return p.HP <= 0;
            }
        ),
        enemies.end()
    );
}
void drawEnemies(){
    // Add in sprites and such.
    for(auto& e : enemies){
        e.draw();
    }
}
void enemyLogic(float dt, Level& lvl, Vector2 playerCenter){
    updateEnemies(dt, lvl, playerCenter);
    drawEnemies();
}
void enemyCollisionCheck(){
    for(auto& e : enemies){
        if(CircleSectorColl(Mradius, Mcenter, e.Hbox, Mdir, MarcSize)){
            e.HP -= 1;
        }
        // Add in bullet coll.
        for(auto& b : bullets){
            if(R2CollCheck(e.Hbox, {b.pos.x, b.pos.y, b.w, b.h})){
                e.HP -= 1;
                b.alive = false;
            }
        }
    }
}
