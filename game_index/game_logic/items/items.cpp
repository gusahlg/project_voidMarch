#include "items.hpp"
#include "../systems/inventory/weapon_factory.hpp"
namespace Weapon{
WeaponSwitch equipped = WeaponSwitch::meleeToggle;

MeleeWeapon sword = ItemSys::createMeleeWeapon(
                                     1, 10, 5,
                                     3, 1, 3, 1,
                                     []{});

RangedWeapon blaster = ItemSys::createRangedWeapon(
                                         1, 10, 5,
                                         3, 1, 3, 1,
                                         5.f, 10.f, 0,
                                         []{});
void loadTextures(){
    sword.setSprite(LoadTexture("assets/graphics/abilities/utilities/equipables/melee/sword.png"));
    blaster.setSprite(LoadTexture("assets/graphics/abilities/utilities/equipables/ranged/blaster.png"));
}
}
