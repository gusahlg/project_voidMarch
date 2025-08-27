#pragma once
#include <cstdint>
enum class screen : std::uint8_t {Main, Select, Game, LevelSelect};
extern screen currentScreen;
