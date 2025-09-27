#include "items.hpp"
namespace Weapon{
WeaponSwitch equipped = WeaponSwitch::meleeToggle;
MeleeWeapon sword = item_sys::createMeleeWeapon(
    LoadTexture("assets/graphics/abilities/utilities/equipables/melee/sword.png"),
    /*dmg      */ 20,
    /*cooldown */ 0.35f,
    /*range    */ 50,
    /*arc      */ 70,
    /*name     */ "Sword"
);
RangedWeapon blaster = item_sys::createRangedWeapon(
    LoadTexture("assets/graphics/abilities/utilities/equipables/melee/blaster.png"),
    /*dmg              */ 20,
    /*cooldown         */ 0.35f,
    /*range            */ 50,
    /*projectile speed */ 70,
    /*name             */ "Blaster"
);
}