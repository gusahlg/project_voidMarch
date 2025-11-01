#include "projectiles.hpp"
#include <vector>
#include "raymath.h"
#include "raylib.h"
#include "../../include/global/scale_utils.hpp"
#include "../../include/global/constants.hpp"
#include "../../include/game/ability_attributes.hpp"
namespace Combat{
bool projActive = false;
void spawnProjectile(Vector2 startpos, Vector2 dir, float w, float h, float speed, int damage){
    projActive = true;
    bullets.emplace_back(
        startpos,
        Vector2Scale(dir, speed),
        w, h, damage
    );
}
void updateProjectiles(Level& lvl, float dt){
    if(!projActive){return;}
    for(auto& b : bullets){
        Rectangle brectPx{
            b.pos.x,
            b.pos.y,
            b.w,
            b.h
        };
        if(CheckCollisionRecs(brectPx, playerRect)){
            b.alive = false;
            continue;
        }
        // Integrate in pixels
        b.pos.x += b.vel.x * dt;
        b.pos.y += b.vel.y * dt;

        Rectangle brectTiles{
            (float)ScaleUtils::toTiles(b.pos.x),
            (float)ScaleUtils::toTiles(b.pos.y),
            (float)ScaleUtils::toTiles(b.w),
            (float)ScaleUtils::toTiles(b.h)
        };
        if(collisionRect(brectTiles.x, brectTiles.y, brectTiles.width, brectTiles.height, lvl)){
            b.alive = false;
            continue;
        }
        DrawRectangleV(b.pos, Vector2{b.w, b.h}, RED);
    }
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const projectile& p){ return !p.alive; }),
        bullets.end()
    );
    projActive = !bullets.empty();
}
}