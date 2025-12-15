#pragma once
#include "../systems/inventory/weapon.hpp"
// Add in new weapons here and define them in the .cpp file
namespace Weapon{
using MeleeWeapon = ItemSys::MeleeWeapon;
using RangedWeapon = ItemSys::RangedWeapon;
extern MeleeWeapon sword;
extern RangedWeapon blaster;
enum class WeaponSwitch {meleeToggle, rangedToggle};
extern WeaponSwitch equipped;
void loadTextures();
}