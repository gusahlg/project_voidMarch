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
// Essential systems used for scaling and communicating constants.
#include "../include/global/constants.hpp"
#include "../include/global/scale_system.hpp"
struct enemy{
    int frames;
    int currentFrame = 0;
    float animDelay;
    float animTimer = 0.f;
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
        switch(currentState){
            case state::Walking:
                animTimer += GetFrameTime();
                if(animTimer > animDelay){
                    currentFrame = (currentFrame + 1) % frames;
                    animTimer = 0.f;
                }
                break;
            case state::Idle:
                animTimer = 0.0f;
                currentFrame = 0;
                break;
            case state::Jumping:
                //Cool things
                break;
        }
        float frameW = (float)tex.width / frames; 
        Rectangle src = {currentFrame * frameW, 0, frameW, (float)tex.height};
        Rectangle dst = {Hbox.x, Hbox.y, frameW*scaleSys.info().scale, tex.height*scaleSys.info().scale};
        DrawTexturePro(tex, src, dst, {0,0}, 0, WHITE);
    }
    enemy(Vector2 pos, int hp, Type t)
    : Hbox{}, MAXHP(hp), HP(hp), kind(t)
{
    const size_t idx = static_cast<size_t>(kind);
    // Constant LUT 
    static constexpr float   speedLUT     [] = {90.f, 20.f, 50.f};
    static constexpr Vector2 sizeLUT      [] = {{10.f,10.f}, {20.f,20.f}, {25.f,25.f}};
    static constexpr float   delayLUT     [] = {30.f, 40.f, 50.f};
    static constexpr float   rangeLUT     [] = {50.f, 100.f, 25.f};
    static constexpr float   animDelayLUT [] = {0.1f, 0.25f, 5.f};
    static constexpr int     framesLUT    [] = {3, 4, 5};
    //Determined values from tables above
    speed     = speedLUT[idx] * scaleSys.info().scale;
    cooldown  = delayLUT[idx] * scaleSys.info().scale;
    range     = rangeLUT[idx] * scaleSys.info().scale;
    animDelay = animDelayLUT[idx];
    frames    = framesLUT[idx];
    // Hitbox calc
    Vector2 sz = { sizeLUT[idx].x * scaleSys.info().scale, sizeLUT[idx].y * scaleSys.info().scale};
    Hbox = { pos.x, pos.y, sz.x, sz.y };

    // Texture determination:
    switch(idx){
        case 0: tex = LoadTexture(""); break;
        case 1: tex = LoadTexture("assets/graphics/enemies/Turtlemaster.png"); break;
        case 2: tex = LoadTexture(""); break;
    }
}
};
std::vector<enemy> enemies;
void spawnEnemy(Vector2 pos, int HP, enemy::Type t){ //Add into level initialization and other stuff.
    enemies.emplace_back(pos, HP, t);
}
void spawnLogic(Vector2 pos, int HP, int ID){
    enemy::Type t;
    switch(ID){
        case(0): t = enemy::Type::generic; break;
        case(1): t = enemy::Type::TurtleMaster; break;
        case(2): t = enemy::Type::Bob; break;
        default: t = enemy::Type::generic; break;
    }
    spawnEnemy(pos, HP, t);
}
// Idea: Add in types, speed and stuff in enemies struct
void updateEnemies(float dt, Level& lvl, Vector2 playerCenter){
    float tileSize = scaleSys.info().tilePx;
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
