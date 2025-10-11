#pragma once
#include <raylib.h>
#include <cstdint>
#include <unordered_set>
#include <cmath>
#include <algorithm>

// Keep everything self-contained and header-only.
// Namespace to avoid collisions with your existing globals.
namespace combat {

// ------------ Small helpers ------------
static inline Vector2 normalizeSafe(Vector2 v) {
    float len2 = v.x*v.x + v.y*v.y;
    if (len2 <= 1e-12f) return Vector2{1.0f, 0.0f};
    float inv = 1.0f / std::sqrt(len2);
    return Vector2{ v.x*inv, v.y*inv };
}
static inline bool sectorHitsAABB(Vector2 center, Vector2 dir, float radiusPx, float fullArcDeg, const Rectangle& enemy)
{
    dir = normalizeSafe(dir);

    // Enemy center + bounding circle radius (half diagonal)
    Vector2 ec{ enemy.x + enemy.width*0.5f, enemy.y + enemy.height*0.5f };
    float enemyR = 0.5f * std::sqrt(enemy.width*enemy.width + enemy.height*enemy.height);

    // Distance gate (expand radius by enemy size)
    float vx = ec.x - center.x;
    float vy = ec.y - center.y;
    float dist2 = vx*vx + vy*vy;
    float effR  = radiusPx + enemyR;
    if (dist2 > effR*effR) return false;

    // Angle gate with conservative widening by enemy angular size
    float len = std::sqrt(std::max(dist2, 1e-12f));
    float halfRad = (fullArcDeg * DEG2RAD) * 0.5f;

    // widen the edge by beta = asin(min(1, enemyR / len))
    float ratio = std::min(1.0f, enemyR / std::max(len, 1e-12f));
    float beta  = std::asin(ratio);
    float cosEdge = std::cos(halfRad + beta);

    float dot = (dir.x*vx + dir.y*vy) / len; // normalized dot to get cos(theta)
    return (dot >= cosEdge);
}

// ------------ Public API ------------

struct Sector {
    Vector2 center{0,0};     // world pixels
    Vector2 dir{1,0};        // will be normalized internally
    float   radius{0.0f};    // pixels
    float   arcDeg{0.0f};    // FULL arc in degrees (e.g., 90 = ±45°)
};

struct AttackState {
    Sector sector{};
    std::uint64_t swingId{0};                 // increments per beginSwing
    std::unordered_set<std::uint64_t> hit;    // enemy keys hit this swing

    // Start a new swing (call once when attack begins)
    void beginSwing(Vector2 center, Vector2 dir, float radius, float fullArcDeg) {
        sector.center = center;
        sector.dir    = normalizeSafe(dir);
        sector.radius = radius;
        sector.arcDeg = fullArcDeg;
        ++swingId;
        hit.clear();
    }

    // Geometry-only test (no marking)
    bool testAABB(const Rectangle& enemy) const {
        return sectorHitsAABB(sector.center, sector.dir, sector.radius, sector.arcDeg, enemy);
    }

    // Geometry + one-hit-per-swing
    // enemyKey: provide a stable id for the enemy (recommended).
    // If you don't have one yet, you can temporarily pass a pointer cast
    // to uint64_t, but prefer a real ID as soon as possible.
    bool testAndMark(const Rectangle& enemy, std::uint64_t enemyKey) {
        if (!testAABB(enemy)) return false;
        auto [it, inserted] = hit.insert(enemyKey);
        return inserted; // true only the first time this swing
    }
};

// Optional: simple debug draw of the current sector
static inline void debugDraw(const AttackState& s) {
    float halfRad = (s.sector.arcDeg * DEG2RAD) * 0.5f;

    // Rotate dir by ±halfRad
    float c = std::cos(halfRad), si = std::sin(halfRad);
    Vector2 d = s.sector.dir;
    Vector2 left  { d.x*c - d.y*si,  d.x*si + d.y*c };
    Vector2 right { d.x*c + d.y*si, -d.x*si + d.y*c };

    Vector2 A{ s.sector.center.x + left.x  * s.sector.radius,
               s.sector.center.y + left.y  * s.sector.radius };
    Vector2 B{ s.sector.center.x + right.x * s.sector.radius,
               s.sector.center.y + right.y * s.sector.radius };

    DrawCircleLines((int)s.sector.center.x, (int)s.sector.center.y, s.sector.radius, Fade(WHITE, 0.25f));
    DrawLineV(s.sector.center, A, GREEN);
    DrawLineV(s.sector.center, B, GREEN);
}

} // namespace vm::combat