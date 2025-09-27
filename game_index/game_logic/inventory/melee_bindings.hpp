#pragma once
#include "raylib.h"
#include <cstdint>
#include <functional>
namespace item_sys {
void bind_enemy_access(void(*forEachEnemy)(const std::function<void(std::uint64_t, Rectangle)>&),
                       void(*damageEnemy)(std::uint64_t, int));
// Start a new melee swing (one call when attack triggers)
void start_melee_swing(Vector2 playerCenter, Vector2 aimWorld,
                       int rangePx, int arcDeg);

// Call every frame while a swing is “active” (or just every frame; it’s cheap).
// Internally ensures “hit once per enemy per swing”.
void resolve_melee_hits(int damage);

// Optional: visualize the sector for debugging.
void draw_melee_debug();

} // namespace item_sys