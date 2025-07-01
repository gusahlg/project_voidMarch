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
    enum dir : std::uint8_t{Up, Down, Left, Right};
    dir currentDir = Up;
    enum state : std::uint8_t{Idle, Walking, Jumping};
    state currentState = Idle;
    void determineState(){
        std::mt19937 rng{
            static_cast<std::mt19937::result_type>(
                std::chrono::steady_clock::now().time_since_epoch().count())
        };
        std::uniform_int_distribution<int> pick(0, 1);   // Change so more alternativs with more abilities
        int choice = pick(rng);
        switch(choice){
            case 0: currentState = Idle;
            case 1: currentState = Walking;
        }

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
        e.determineState();
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
    for(const auto& e : enemies){
        DrawRectangleRec(e.Hbox, RED);
    }
}
void enemyLogic(float dt, Level& lvl){
    updateEnemies(dt, lvl);
    drawEnemies();
}
void enemyCollisionCheck(){
    attacking = true;
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
