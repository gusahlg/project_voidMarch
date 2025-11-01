#include "weapon.hpp"
namespace ItemSys {
inline RangedWeapon createRangedWeapon(
                                       int damage, float cooldownSec, int range,
                                       int columns, int rows, int frames, int animDelay,
                                       float projW, float projH, float projSpeed,
                                       std::function<void()> fx = {}){
    RangedWeapon u{damage, cooldownSec, range, std::move(fx),
                   projW, projH, projSpeed};
    u.setSpriteSheet(columns, rows, frames, animDelay);
    return u;
}

inline MeleeWeapon createMeleeWeapon(
                                     int damage, float cooldownSec, int range,
                                     int columns, int rows, int frames, int animDelay,
                                     std::function<void()> fx = {}){
    MeleeWeapon u{damage, cooldownSec, range, std::move(fx)};
    u.setSpriteSheet(columns, rows, frames, animDelay);
    return u;
}

inline Weapon createWeapon(
                            int damage, float cooldownSec, int range,
                            int columns, int rows, int frames, int animDelay,
                            std::function<void()> fx = {}){
    Weapon u{damage, cooldownSec, range, std::move(fx)};
    u.setSpriteSheet(columns, rows, frames, animDelay);
    return u;
}
inline Utility<Default> createUtility(
                            int damage, float cooldownSec, int range,
                            int columns, int rows, int frames, int animDelay,
                            std::function<void()> fx = {}){
    Utility<Default> u{damage, cooldownSec, range, std::move(fx)};
    u.setSpriteSheet(columns, rows, frames, animDelay);
    return u;
}
}