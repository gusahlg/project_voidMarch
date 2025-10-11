#pragma once
#include "../inventory/weapon.hpp"
// Add in new weapons here and define them in the .cpp file
namespace Weapon{
using MeleeWeapon = item_sys::MeleeWeapon;
using RangedWeapon = item_sys::RangedWeapon;
extern MeleeWeapon sword;
extern RangedWeapon blaster;
enum class WeaponSwitch {meleeToggle, rangedToggle};
extern WeaponSwitch equipped;
void loadTextures();
}