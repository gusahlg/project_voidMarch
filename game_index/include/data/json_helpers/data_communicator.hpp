#pragma once
#include <array>
#include <cstdint>
#include <algorithm>
/* PLAN: 
I make a system that looks for the number of sections in the json file
being loaded then the number of them get stored here with their values
then I can use this to 
*/
enum class Stat : uint8_t {HP, MP, STR, AGI, INT, _COUNT};
struct Stats{
    // So that the size adapts to the number of enum values.
    static constexpr size_t COUNT = static_cast<size_t>(Stat::_COUNT);
    // Array that stores states, values zero initialize.
    std::array<int, COUNT> values{}; 
    // You can get the stat of your choice.
    int get(Stat s) const{
        return values[static_cast<size_t>(s)];
    }
    // Sets the a stat to an int of your choice.
    void set(Stat s, int n){
        values[static_cast<size_t>(s)] = n;
    }
    void damage(int dmg){
        values[0] = values[0] - dmg;
    }
    bool isDead() const{
        return get(Stat::HP) <= 0;
    }
};

