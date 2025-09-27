#include <raylib.h>
#include <iostream>
#include "../include/game/loadGame.hpp"
#include "../include/game/ability_attributes.hpp"
#include "../include/game/enemy_data.hpp"
#include <vector>
#include <cmath>
#include <raymath.h>
#include <algorithm>
#include "../game_logic/inventory/melee_bindings.hpp"
// Essential systems used for scaling and communicating constants.
#include "../include/global/constants.hpp"
#include "../include/global/scale_system.hpp"
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
//Weapons and their abilities are defined bellow.
std::vector<projectile> bullets;
void spawnProjectile(Vector2 startpos, Vector2 dir, float w, float h, float speed, bool enemyOwner, int damage){
    projActive = true;
    bullets.emplace_back(
        startpos,
        Vector2Scale(dir, speed),
        w, h, enemyOwner, damage
    );
}
void updateProjectiles(Level& lvl, float dt){
    if(bullets.empty()){
        projActive = false;
        return;
    }
    auto& si = scaleSys.info();
    for(auto& b : bullets){
        if (!b.alive) continue;
        b.pos.x += b.vel.x * dt;
        b.pos.y += b.vel.y * dt;
        Rectangle brect{
            b.pos.x, b.pos.y,
            toTiles((b.w * si.scale), si),
            toTiles((b.h * si.scale), si)
        };
        if(collisionRect(brect.x, brect.y, brect.width, brect.height, lvl)){
            b.alive = false;
            continue;
        }
        if(!b.enemyOwner && b.alive){
            item_sys::for_each_enemy([&](std::uint64_t id, Rectangle enemyAABB){
                if(!b.alive) return; 
                if(CheckCollisionRecs(brect, enemyAABB)) {
                    item_sys::damage_enemy(id, b.damage);
                    b.alive = false;  
                }
            });
        }

        // TODO: if (b.enemyOwner) handle player collision here
    }

    // Remove dead bullets
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const projectile& p) { return !p.alive; }),
        bullets.end()
    );
    projActive = !bullets.empty();
}
void drawProjectiles(){
    for(const auto& b : bullets){
        auto& si = scaleSys.info();
        float gw = toTiles((b.w*si.scale), si);
        float gh = toTiles((b.h*si.scale), si);
        DrawRectangle(b.pos.x, b.pos.y, gw, gh, RED);
    }
}
void updateRangedAttack(Vector2 pos, Vector2 dir, float projW, float projH, float projSpeed, float dt, Level& lvl){
    updateProjectiles(lvl, dt);
    drawProjectiles();
}
Rectangle Esrc = {0, 0, 32, 32};