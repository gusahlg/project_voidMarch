#pragma once
#include "../nlohmann/json.hpp"
#include <fstream>
#include <string_view>
// saving and loading.
std::size_t value_count_reader(std::string_view path){
    std::ifstream data{std::string(path)};
    nlohmann::json j;
    data >> j;
    return j.size();
}
template<class T>
inline void save_json(std::string_view file, T& data){
    nlohmann::json j = nlohmann::json::array();
    dest.COUNT = value_count_reader(file);
    // Load into json format
    for(size_t i = 0; i < data.COUNT; ++i){
        j[i] = data.values[i];
    }
    // Dump json into the json file
    std::ofstream(file.data()) << j.dump(2);
}
// Requires a values and COUNT member.
template<class T>
inline T load_json(std::string_view file){
    T dest;
    dest.COUNT = value_count_reader(file);
    if(std::ifstream f{file.data()}){
        nlohmann::json j;
        f >> j;
        for(size_t i = 0; i < dest.COUNT; ++i){
            dest.values[i]=j[i].get<int>();
        }
    }
    return dest;
}
