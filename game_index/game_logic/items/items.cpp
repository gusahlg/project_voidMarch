#include "items.hpp"
namespace Weapon{
WeaponSwitch equipped = WeaponSwitch::meleeToggle;

MeleeWeapon sword = item_sys::createMeleeWeapon(
    Texture2D{},
    /*dmg      */ 20,
    /*cooldown */ 0.2f,
    /*range    */ 50,
    /*arc      */ 80,
    /*name     */ "Sword"
);
RangedWeapon blaster = item_sys::createRangedWeapon(
    Texture2D{},
    /*dmg              */ 20,
    /*cooldown         */ 0.2,
    /*range            */ 50,
    /*projectile speed */ 1,
    /*name             */ "Blaster"
);
void loadTextures(){
    sword.setSprite(LoadTexture("assets/graphics/abilities/utilities/equipables/melee/sword.png"));
    blaster.setSprite(LoadTexture("assets/graphics/abilities/utilities/equipables/ranged/blaster.png"));
    sword.setSpriteSheet(3, 1, 0, {1,2}, 0.175f);
    blaster.setSpriteSheet(3, 1, 0, {1,2}, 1.0f);
}
}