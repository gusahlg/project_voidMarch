#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>
#include <utility>
#include <vector>
#include <cmath>
// This is for defining items.
namespace item_sys {
class Weapon {
public:
    virtual ~Weapon() = default;

    // Copying small stat-only objects by value is fine in your current usage.
    Weapon(const Weapon&) = default;
    Weapon& operator=(const Weapon&) = default;
    Weapon(Weapon&&) = default;
    Weapon& operator=(Weapon&&) = default;

    // Construction with core stats (no textures loaded here).
    Weapon(Texture2D spriteSheet,
           int dmg,
           float attackIntervalSec,
           int rangeFromPlayerPx,
           std::string weaponName)
    : damage_(dmg)
    , attackCooldown_(attackIntervalSec)
    , timer_(0.0f)                      // first click fires immediately
    , sprite_(spriteSheet)
    , rangePx_(rangeFromPlayerPx)
    , name_(std::move(weaponName))
    {}

    // Cooldown gate. Returns true if an attack may start this frame.
    bool attackReady(float dt) {
        timer_ -= dt;
        if (timer_ > 0.0f) return false;
        timer_ = attackCooldown_;
        return true;
    }

    // --- Animation control (idle + attack strip) ---
    // Treat spritesheet as (cols x rows). Use flattened frame indices (row*cols+col).
    void setSpriteSheet(int cols, int rows, int idleFrame,
                        std::vector<int> attackFrames,
                        float attackDurationSec)
    {
        sheetCols_ = cols > 0 ? cols : 1;
        sheetRows_ = rows > 0 ? rows : 1;
        idleFrame_ = idleFrame >= 0 ? idleFrame : 0;
        attackFrames_ = std::move(attackFrames);
        attackAnimDuration_ = attackDurationSec > 0.f ? attackDurationSec : 0.f;
    }

    void beginAttackAnim() {
        if (!attackFrames_.empty() && attackAnimDuration_ > 0.f) {
            animT_ = attackAnimDuration_;
            animFrameIdx_ = 0;
        }
    }

    // Call per-frame.
    void tickAnim(float dt) {
        if (animT_ <= 0.f || attackFrames_.empty() || attackAnimDuration_ <= 0.f) return;
        float elapsed = attackAnimDuration_ - animT_;
        float per = attackAnimDuration_ / static_cast<float>(attackFrames_.size());
        int idx = per > 0.f ? static_cast<int>(elapsed / per) : 0;
        if (idx >= static_cast<int>(attackFrames_.size())) idx = static_cast<int>(attackFrames_.size()) - 1;
        animFrameIdx_ = idx;
        animT_ -= dt;
        if (animT_ < 0.f) animT_ = 0.f;
    }

    // Draw (non-rotating). Melee uses this.
    virtual void draw(Vector2 playerCenter, Vector2 /*aimWorld*/, float scale = 1.0f) const {
        if (sprite_.id == 0) return;
        Rectangle src = currentFrameSrc();
        if (src.width <= 0 || src.height <= 0) return;
        Rectangle dst{ playerCenter.x, playerCenter.y, src.width * scale, src.height * scale };
        Vector2 origin{ dst.width * 0.5f, dst.height * 0.5f };
        DrawTexturePro(sprite_, src, dst, origin, /*rotation*/ 0.0f, WHITE);
    }

    // Lightweight accessors
    int         damage()   const { return damage_; }
    float       cooldown() const { return attackCooldown_; }
    int         range()    const { return rangePx_; }
    const char* name()     const { return name_.c_str(); }
    Texture2D   sprite()   const { return sprite_; }

    // If you load textures elsewhere and want to swap at runtime:
    void setSprite(Texture2D t) { sprite_ = t; }

protected:
    Rectangle currentFrameSrc() const {
        if (sprite_.id == 0) return Rectangle{0,0,0,0};
        int fw = sprite_.width  / sheetCols_;
        int fh = sprite_.height / sheetRows_;
        if (fw <= 0 || fh <= 0) return Rectangle{0,0,(float)sprite_.width,(float)sprite_.height};
        int frameIndex = (animT_ > 0.f && !attackFrames_.empty()) ? attackFrames_[animFrameIdx_] : idleFrame_;
        int row = frameIndex / sheetCols_;
        int col = frameIndex % sheetCols_;
        return Rectangle{ (float)(col*fw), (float)(row*fh), (float)fw, (float)fh };
    }

private:
    // Immutable stats
    const int         damage_;
    const float       attackCooldown_;
    const int         rangePx_;
    const std::string name_;

    // Mutable runtime
    float             timer_;      // cooldown timer (seconds)

    // Visuals
    Texture2D         sprite_;

    // Spritesheet + animation
    int sheetCols_ = 1;
    int sheetRows_ = 1;
    int idleFrame_ = 0;
    std::vector<int> attackFrames_{};
    float attackAnimDuration_ = 0.0f;
    float animT_ = 0.0f;
    int   animFrameIdx_ = 0;
};

// -----------------------------------------------------------------------------
// MeleeWeapon: adds arc degrees; inherits non-rotating draw
// -----------------------------------------------------------------------------
class MeleeWeapon final : public Weapon {
public:
    MeleeWeapon(Texture2D spriteSheet,
                int dmg,
                float attackIntervalSec,
                int rangeFromPlayerPx,
                int attackArcDegrees,
                std::string weaponName)
    : Weapon(spriteSheet, dmg, attackIntervalSec, rangeFromPlayerPx, std::move(weaponName))
    , arcDeg_(attackArcDegrees)
    {}

    int arcDegrees() const { return arcDeg_; }

private:
    const int arcDeg_;
};

// -----------------------------------------------------------------------------
// RangedWeapon: adds projectile speed; overrides draw to rotate toward mouse
// -----------------------------------------------------------------------------
class RangedWeapon final : public Weapon {
public:
    RangedWeapon(Texture2D spriteSheet,
                 int dmg,
                 float attackIntervalSec,
                 int rangeFromPlayerPx,
                 float projectileSpeed,
                 std::string weaponName)
    : Weapon(spriteSheet, dmg, attackIntervalSec, rangeFromPlayerPx, std::move(weaponName))
    , projSpeed_(projectileSpeed)
    {}

    float projectileSpeed() const { return projSpeed_; }

    // Rotate only for ranged
    void draw(Vector2 playerCenter, Vector2 aimWorld, float scale = 1.0f) const override {
        if (sprite().id == 0) return;
        Rectangle src = currentFrameSrc();
        if (src.width <= 0 || src.height <= 0) return;
        Rectangle dst{ playerCenter.x, playerCenter.y, src.width * scale, src.height * scale };
        Vector2 origin{ dst.width * 0.5f, dst.height * 0.5f };
        Vector2 dir = Vector2Subtract(aimWorld, playerCenter);
        float ang = (dir.x || dir.y) ? (atan2f(dir.y, dir.x) * (180.0f / 3.14159265f)) : 0.f;
        if(dir.x < 0){src={src.x, -src.y, src.width, -src.height};}
        DrawTexturePro(sprite(), src, dst, origin, ang, WHITE);
    }

private:
    const float projSpeed_;
};

// -----------------------------------------------------------------------------
// Tiny factories (return concrete types by value)
// -----------------------------------------------------------------------------
inline MeleeWeapon createMeleeWeapon(Texture2D sprite, int dmg, float cdSec,
                                     int rangePx, int arcDeg, std::string name)
{
    return MeleeWeapon(sprite, dmg, cdSec, rangePx, arcDeg, std::move(name));
}

inline RangedWeapon createRangedWeapon(Texture2D sprite, int dmg, float cdSec,
                                       int rangePx, float speed, std::string name)
{
    return RangedWeapon(sprite, dmg, cdSec, rangePx, speed, std::move(name));
}

} // namespace item_sys
