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
struct enemy{
    Rectangle Hbox;
    int HP;
    enum dir : std::uint8_t{Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight};
    dir currentDir = Up;
    enum state : std::uint8_t{Idle, Walking, Jumping};
    state currentState = Idle;
    void determineState(const float range){
        Vector2 Emid = {Hbox.x + Hbox.width/2.0f, Hbox.y + Hbox.height/2.0f};
        bool inRangeX = std::fabs(Emid.x - playerPixCenter.x) <= range;
        bool inRangeY = std::fabs(Emid.y - playerPixCenter.y) <= range;
        float distX = Emid.x < playerPixCenter.x ? playerPixCenter.x - Emid.x : Emid.x - playerPixCenter.x;
        float distY = Emid.y < playerPixCenter.y ? playerPixCenter.y - Emid.y : Emid.y - playerPixCenter.y;
        // Add in randomness and such for good pathfinding current system is temporary
        if(inRangeX && inRangeY){
            currentState = Walking;
        //Check if diagonals are faster, then left/right, up/down.
            if(std::sqrtf(distX * distX + distY * distY) < distX && std::sqrtf(distX * distX + distY * distY) < distY){
                bool right = distX < playerPixCenter.x;
                bool down = distY < playerPixCenter.y;
                if(right && !down) currentDir = dir::UpRight;
                else if(right && down) currentDir = dir::DownRight;
                else if(!right && !down) currentDir = dir::UpLeft;
                else currentDir = dir::DownLeft;
            } 
            else if(distX < distY) currentDir = Emid.x < playerPixCenter.x ? dir::Left : dir::Right;
            else currentDir = Emid.y < playerPixCenter.y ? dir::Up : dir::Down;
        }
        else currentState = Idle;

    }
    enemy(Rectangle Hbox, int HP)
    : Hbox(Hbox), HP(HP) {}
};
std::vector<enemy> enemies;
void spawnEnemy(Vector2 pos, float w, float h, int HP){ //Add into level initialization and other stuff.
    Rectangle Hbox = {pos.x, pos.y, w, h};
    enemies.emplace_back(Hbox, HP);
}
void updateEnemies(float dt, Level& lvl){
    for(auto& e : enemies){
        e.determineState(100.0f * scale);
        auto f = e;
        switch(e.currentState){
            case(e.Walking):
                f.Hbox.x -= 20;
                break;
            case(e.Jumping):
                //Jump
                break;
            default:
                
                break;
        }
        if(!collisionRect(e.Hbox.x, e.Hbox.y, e.Hbox.width, e.Hbox.height, lvl)){
            e.Hbox = f.Hbox;
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
