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
#include <functional>
// Weapon and Inventory.

// Essential systems used for scaling and communicating constants.
#include "../include/global/constants.hpp"
#include "../include/global/scale_utils.hpp"
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
    for(auto& e : enemies){
        e.update(playerCenter);
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
        if(!collisionRect((float)ScaleUtils::toTiles(f.x), (float)ScaleUtils::toTiles(f.y), (float)ScaleUtils::toTiles(f.width), (float)ScaleUtils::toTiles(f.height), lvl)){
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
