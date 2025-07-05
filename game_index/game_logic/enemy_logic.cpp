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
struct enemy{
    Rectangle Hbox;
    int HP;
    enum dir : std::uint8_t{Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight};
    dir currentDir = Up;
    enum state : std::uint8_t{Idle, Walking, Jumping};
    state currentState = Idle;
    enum class Type : std::uint8_t{generic, TurtleMaster, Bob}; //Expand for more variations
    Type kind = Type::generic;
    Texture2D sprite; //Good for later
    // Bellow depends on type.
    float speed{};
    void determineState(const float range){
        Vector2 Emid = {Hbox.x + Hbox.width/2.0f, Hbox.y + Hbox.height/2.0f};
        bool inRangeX = std::fabs(Emid.x - playerPixCenter.x) <= range;
        bool inRangeY = std::fabs(Emid.y - playerPixCenter.y) <= range;
        float distX = Emid.x < playerPixCenter.x ? playerPixCenter.x - Emid.x : Emid.x - playerPixCenter.x;
        float distY = Emid.y < playerPixCenter.y ? playerPixCenter.y - Emid.y : Emid.y - playerPixCenter.y;
        float dx = std::fabs(playerPixCenter.x - Emid.x);
        float dy = std::fabs(playerPixCenter.y - Emid.y);
        // Add in randomness and such for good pathfinding current system is temporary
        if(inRangeX && inRangeY){
            currentState = state::Walking;
        }
        else currentState = state::Idle;
        //Check if diagonals are faster, then left/right, up/down.
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
    enemy(Rectangle Hbox, int HP, Type t = Type::generic)
    : Hbox(Hbox), HP(HP), kind(t) {
    static constexpr float speedLUT[] = {10.0f, 20.0f, 30.0f};
    speed = speedLUT[static_cast<uint8_t>(kind)];
    }
};
std::vector<enemy> enemies;
void spawnEnemy(Vector2 pos, float w, float h, int HP, enemy::Type t = enemy::Type::generic){ //Add into level initialization and other stuff.
    Rectangle Hbox = {pos.x, pos.y, w, h};
    enemies.emplace_back(Hbox, HP, t);
}
void spawnLogic(Vector2 pos, float w, float h, int HP, int ID){
    enemy::Type t;
    switch(ID){
        case(0): t = enemy::Type::generic;
        case(1): t = enemy::Type::TurtleMaster;
        case(2): t = enemy::Type::Bob;
    }
    spawnEnemy(pos, w, h, 10, t);
}
// Idea: Add in types, speed and stuff in enemies struct
void updateEnemies(float dt, Level& lvl){
    for(auto& e : enemies){
        e.determineState(100.0f * scale);
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
                    case enemy::UpLeft:    f.x -= step; f.y -= step; break;
                    case enemy::UpRight:   f.x += step; f.y -= step; break;
                    case enemy::DownLeft:  f.x -= step; f.y += step; break;
                    case enemy::DownRight: f.x += step; f.y += step; break;
                }
                break;
            }
            case(enemy::Jumping):
                //Jump
                break;
            default:
                
                break;
        }
        if(!collisionRect(f.x, f.y, f.width, f.height, lvl)){
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
    for(const auto& e : enemies){
        DrawRectangleRec(e.Hbox, RED);
    }
}
void enemyLogic(float dt, Level& lvl){
    updateEnemies(dt, lvl);
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
