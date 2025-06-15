#include <raylib.h>
#include <iostream>
#include "../include/game/loadGame.hpp"
#include "../include/game/ability_attributes.hpp"
#include <vector>
#include <cmath>
#include <raymath.h>
#include <algorithm>
struct enemy{
    Rectangle Hbox;
    int HP;
    enum dir{Up, Down, Left, Right};
    enum state{Idle, Walking, Jumping};
    state currentState = Idle;
    void determineState(){
        // Add in detection of some sort. Also add in direction calc.
        state currentState = Idle;
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
                //Walk
            case(e.Jumping):
                //Jump
            default:
                
                break;
        }
        if(collisionRect(e.Hbox.x, e.Hbox.y, e.Hbox.width, e.Hbox.height, lvl)){
            e.Hbox = f.Hbox;
        }
    }
    enemies.erase(
        std::remove_if(
            enemies.begin(), enemies.end(), [](const enemy& p){
                return p.HP <= 0;
            }
        ),
        enemies.end()
    );
}
void drawEnemies(){
    for(const auto& e : enemies){
        DrawRectangleRec(e.Hbox, BLUE);
    }
}
void enemyLogic(float dt, Level& lvl){
    updateEnemies(dt, lvl);
    drawEnemies();
}
