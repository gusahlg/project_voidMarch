#pragma once
#include "player_data.hpp"
#include "../nlohmann/json.hpp"
#include <fstream>
#include <string_view>
inline void save(const Stats& s, std::string_view file){
    nlohmann::json j;
    for(size_t i = 0; i < Stats::COUNT; ++i){
        j[i] = s.values[i];
    }
    std::ofstream(file.data()) << j.dump(2);
}
inline Stats load(std::string_view file){
    Stats s;
    if(std::ifstream f{file.data()}){ 
        nlohmann::json j;
        f >> j;
        for(size_t i = 0; i < Stats::COUNT; ++i){
            s.values[i]=j[i].get<int>();
        }
    }
    else s.values = {
        100, 30, 12, 14, 8
    };
    return s;
}
// saving and loading.