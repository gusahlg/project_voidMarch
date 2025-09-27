#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>
#include <utility>
#include <cmath> 

namespace item_sys {
class Weapon {
public:
    virtual ~Weapon() = default;
    Weapon(const Weapon&) = default;
    Weapon& operator=(const Weapon&) = default;

    bool attackReady(float dt) {
        timer_ -= dt;
        if (timer_ > 0.0f) return false;
        timer_ = attackCooldown_;
        return true;
    }
    virtual void attack(Vector2 playerCenter, Vector2 aimWorld) = 0;
    virtual void draw(Vector2 playerCenter, Vector2 aimWorld, float scale = 1.0f) const{
        if(sprite_.id == 0) return; // texture not loaded
        const int frameW = sprite_.width;   // if it's a single image; else sprite_.width / 3
        const int frameH = sprite_.height;
        Rectangle src{0, 0, (float)frameW, (float)frameH};
        Rectangle dst{
            playerCenter.x, playerCenter.y,
            frameW * scale, frameH * scale
        };
        Vector2 dir = Vector2Subtract(aimWorld, playerCenter);
        float angleDeg = 0.0f;
        if (dir.x != 0.0f || dir.y != 0.0f) angleDeg = atan2f(dir.y, dir.x) * (180.0f / 3.14159265f);
        Vector2 origin{ dst.width * 0.5f, dst.height * 0.5f };
        DrawTexturePro(sprite_, src, dst, origin, angleDeg, WHITE);
    }
    int         damage()   const { return damage_; }
    float       cooldown() const { return attackCooldown_; }
    int         range()    const { return range_; }
    const char* name()     const { return name_.c_str(); }
    Texture2D   sprite_;
protected:
    Weapon(Texture2D spriteSheet, int dmg, float attackInterval, int rangeFromPlayer, std::string weaponName)
    : damage_(dmg)
    , attackCooldown_(attackInterval)
    , timer_(0.0f)               // fire immediately on first press
    , sprite_(spriteSheet)
    , range_(rangeFromPlayer)
    , name_(std::move(weaponName))
    {}
    int dmg()      const { return damage_; }
    float cd()       const { return attackCooldown_; }
    int reach()    const { return range_; }
    const std::string& id() const { return name_; }
private:
    const int         damage_;
    const float       attackCooldown_;
    float             timer_;
    const int         range_;
    const std::string name_;
};
class RangedWeapon : public Weapon {
public:
    RangedWeapon(Texture2D spriteSheet, int dmg, float attackInterval,
                 int rangeFromPlayer, float projectileSpeed, std::string weaponName)
    : Weapon(spriteSheet, dmg, attackInterval, rangeFromPlayer, std::move(weaponName)), projectileSpeed_(projectileSpeed){}

    void attack(Vector2 playerCenter, Vector2 aimWorld) override {
        Vector2 dir = Vector2Subtract(aimWorld, playerCenter);
        float len2 = dir.x*dir.x + dir.y*dir.y;
        if (len2 > 1e-8f) {
            float inv = 1.0f / sqrtf(len2);
            dir = {dir.x*inv, dir.y*inv};
        } else {
            dir = {1.f, 0.f};
        }
        (void)dir;
    }
    float projectileSpeed() const { return projectileSpeed_; }
private:
    const float projectileSpeed_;
};
class MeleeWeapon : public Weapon {
public:
    MeleeWeapon(Texture2D spriteSheet, int dmg, float attackInterval,
                int rangeFromPlayer, int attackArcDegrees, std::string weaponName)
    : Weapon(spriteSheet, dmg, attackInterval, rangeFromPlayer, std::move(weaponName)), arcDeg_(attackArcDegrees){}
    void attack(Vector2 playerCenter, Vector2 aimWorld) override {
        Vector2 dir = Vector2Subtract(aimWorld, playerCenter);
        float len2 = dir.x*dir.x + dir.y*dir.y;
        if (len2 > 1e-8f) {
            float inv = 1.0f / sqrtf(len2);
            dir = {dir.x*inv, dir.y*inv};
        } else {
            dir = {1.f, 0.f};
        }
        (void)dir;
    }
    int arcDegrees() const { return arcDeg_; }
private:
    const int arcDeg_;
};
inline RangedWeapon createRangedWeapon(Texture2D sprite, int dmg, float cdSec,
                                       int rangePx, float projectileSpeed, std::string name)
{
    return RangedWeapon(sprite, dmg, cdSec, rangePx, projectileSpeed, std::move(name));
}

inline MeleeWeapon createMeleeWeapon(Texture2D sprite, int dmg, float cdSec,
                                     int rangePx, int arcDeg, std::string name)
{
    return MeleeWeapon(sprite, dmg, cdSec, rangePx, arcDeg, std::move(name));
}

} // namespace item_sys