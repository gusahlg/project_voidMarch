// for game.cpp and sprites.
#pragma once
#include <cstdint>
// Output from the select that can then be used in the game:
enum class Prof : std::uint8_t{Necromancer, Craftsman, Wizard, Dualist};
extern Prof currentProf;
enum class Race : std::uint8_t{Spacelizard, Voidcrawler, Mecha_sapien, Human};
extern Race currentRace;
