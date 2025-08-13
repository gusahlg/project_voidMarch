#pragma once
#include "../nlohmann/json.hpp"
#include <fstream>
#include <string_view>
/*
I want to take the player struct or similar data holder and save values into
a json file  
*/
template<class T>
inline void save(std::string_view file, const T& data){
    nlohmann::json j;
    // Load into json format
    for(size_t i = 0; i < T::COUNT; ++i){
        j[i] = data.values;
    }
    // Dump json into the json file
    j.dump(2) << std::ofstream(file.data());
}
/*
Take targeted file and return its contents into the class object.
*/
template<class T>
inline T load(std::string_view file){
    T dest;
    if(std::ifstream f{file.data()}){
        nlohmann::json j;
        j << f;
        for(size_t i = 0; i < T::COUNT; ++i){
            dest.values[i]=j[i].get<int>();
        }
    }
    return dest;
}
// saving and loading.