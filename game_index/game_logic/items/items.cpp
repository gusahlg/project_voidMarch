#include "items.hpp"
namespace Weapon{
WeaponSwitch equipped = WeaponSwitch::meleeToggle;

MeleeWeapon sword = item_sys::createMeleeWeapon(
    Texture2D{},
    /*dmg      */ 20,
    /*cooldown */ 0.35f,
    /*range    */ 50,
    /*arc      */ 70,
    /*name     */ "Sword"
);
RangedWeapon blaster = item_sys::createRangedWeapon(
    Texture2D{},
    /*dmg              */ 20,
    /*cooldown         */ 0.35f,
    /*range            */ 50,
    /*projectile speed */ 70,
    /*name             */ "Blaster"
);
void loadTextures(){
    sword.sprite_ = LoadTexture("assets/graphics/abilities/utilities/equipables/melee/sword.png");
    blaster.sprite_ = LoadTexture("assets/graphics/abilities/utilities/equipables/ranged/blaster.png");
}
}