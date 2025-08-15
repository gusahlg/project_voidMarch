#pragma once
#include "../nlohmann/json.hpp"
#include <fstream>
#include <string_view>
// saving and loading.
template<class T>
inline void save_json(std::string_view file, const T& data){
    nlohmann::json j;
    // Load into json format
    for(size_t i = 0; i < T::COUNT; ++i){
        j[i] = data.values;
    }
    // Dump json into the json file
    std::ofstream(file.data()) >> j.dump(2);
}
// Requires a values and COUNT member.
template<class T>
inline T load_json(std::string_view file){
    T dest;
    if(std::ifstream f{file.data()}){
        nlohmann::json j;
        f >> j;
        for(size_t i = 0; i < dest.COUNT; ++i){
            dest.values[i]=j[i].get<int>();
        }
    }
    return dest;
}
