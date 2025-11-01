#include "raylib.h"
#include <vector>
#include "raymath.h"
#include "../../include/game/ability_attributes.hpp"
namespace Combat{
struct projectile {
    Vector2 pos;
    Vector2 vel;
    float w, h;
    bool alive;
    int damage;
    projectile(Vector2 p, Vector2 v, float w_, float h_, int dmg)
      : pos(p), vel(v), w(w_), h(h_), alive(true), damage(dmg) {}
};
inline std::vector<projectile> bullets;
void spawnProjectile(Vector2 startpos, Vector2 dir, float w, float h, float speed, int damage);
void updateProjectiles(Level& lvl, float dt);
}