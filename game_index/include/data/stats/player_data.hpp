// This is for handling different player states, stats (two similar words after each other, don't fall asleep my fellow 'stay up late gamedev') such as health and attributes such as 'intelligence'.
#pragma once
#include <array>
#include <cstdint>
#include <algorithm>
enum class Stat : uint8_t { HP, MP, STR, AGI, INT, _COUNT };
struct Stats {
    static constexpr size_t COUNT = static_cast<size_t>(Stat::_COUNT);
    std::array<int, COUNT> v{}; 
    int  get(Stat s)  const { return v[static_cast<size_t>(s)]; }
    void set(Stat s,int n)  { v[static_cast<size_t>(s)] = n;   }
    void damage(int dmg){ v[0] = std::max(0, get(Stat::HP)-dmg);} // 0 == HP
    bool isDead() const { return get(Stat::HP) == 0; }
};
// Helpers for checking states and the basic structure of the stat system.

