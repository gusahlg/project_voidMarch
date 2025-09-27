#include "items.hpp"
namespace Weapon{
static MeleeWeapon sword = item_sys::createMeleeWeapon(
    LoadTexture("assets/graphics/abilities/utilities/equipables/melee/sword.png"),
    /*dmg      */ 20,
    /*cooldown */ 0.35f,
    /*range    */ 50,
    /*arc      */ 70,
    /*name     */ "Sword"
);
static RangedWeapon blaster = item_sys::createRangedWeapon(
    LoadTexture("assets/graphics/abilities/utilities/equipables/melee/sword.png"),
    /*dmg              */ 20,
    /*cooldown         */ 0.35f,
    /*range            */ 50,
    /*projectile speed */ 70,
    /*name             */ "Blaster"
);
}