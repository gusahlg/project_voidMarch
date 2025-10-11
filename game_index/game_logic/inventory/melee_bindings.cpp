#include "melee_bindings.hpp"
#include "sector_hit.hpp"
#include <vector>
#include "raymath.h"
#include <functional>

// If you have a central enemy API later, include it here instead of forward hooks.

namespace item_sys {
namespace {
    combat::AttackState g_melee; // lives here, not in a header
    // These two function pointers are tiny hooks that game.cpp will set:
    using ForEachEnemyFn = void(*)(const std::function<void(std::uint64_t /*id*/, Rectangle /*aabb*/)> &);
    using DamageEnemyFn  = void(*)(std::uint64_t /*id*/, int /*damage*/);

    ForEachEnemyFn  g_forEachEnemy = nullptr;
    DamageEnemyFn   g_damageEnemy  = nullptr;
}

// Public: let game.cpp provide enemy accessors once on startup.
void bind_enemy_access(ForEachEnemyFn forEachEnemy, DamageEnemyFn damageEnemy) {
    g_forEachEnemy = forEachEnemy;
    g_damageEnemy  = damageEnemy;
}

void start_melee_swing(Vector2 playerCenter, Vector2 aimWorld, int rangePx, int arcDeg) {
    if (!g_forEachEnemy || !g_damageEnemy) return; // not bound yet
    Vector2 dir = Vector2Subtract(aimWorld, playerCenter);
    g_melee.beginSwing(playerCenter, dir, (float)rangePx, (float)arcDeg);
}

void resolve_melee_hits(int damage) {
    if (!g_forEachEnemy || !g_damageEnemy) return; // not bound yet

    // Pull current sector snapshot
    combat::Sector s = g_melee.sector;

    g_forEachEnemy([&](std::uint64_t id, Rectangle aabb){
        // Only test if not already hit this swing
        if (g_melee.hit.find(id) != g_melee.hit.end()) return;
        if (combat::sectorHitsAABB(s.center, s.dir, s.radius, s.arcDeg, aabb)) {
            g_damageEnemy(id, damage);
            g_melee.hit.insert(id);
        }
    });
}

void draw_melee_debug() {
    combat::debugDraw(g_melee);
}
bool for_each_enemy(const std::function<void(std::uint64_t, Rectangle)>& f) {
    if (!g_forEachEnemy) return false;
    g_forEachEnemy(f);
    return true;
}
void damage_enemy(std::uint64_t id, int damage) {
    if (!g_damageEnemy) return;
    g_damageEnemy(id, damage);
}

} // namespace item_sys