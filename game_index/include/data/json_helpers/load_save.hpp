#pragma once
#include "../nlohmann/json.hpp"
#include <fstream>
#include <string>
#include <string_view>
// saving and loading.
inline std::size_t value_count_reader(std::string_view path){
    std::ifstream data{std::string(path)};
    nlohmann::json j;
    data >> j;
    return j.is_array() ? j.size() : 0;
}
template<class T>
inline void save_json(std::string_view file, T& data){
    nlohmann::json j = nlohmann::json::array();
    data.COUNT = value_count_reader(file);
    // Load into json format
    for(std::size_t i = 0; i < data.COUNT && i < data.values.size(); ++i){
        j.push_back(data.values[i]);
    }
    // Dump json into the json file
    std::ofstream out{std::string(file)};
    out << j.dump(2);
}
// Requires a values and COUNT member.
template<class T>
inline T load_json(std::string_view file){
    T dest{};
    dest.COUNT = value_count_reader(file);
    std::ifstream in{std::string(file)};
    nlohmann::json j;
    in >> j;
    dest.values.clear();
    dest.values.resize(dest.COUNT, 0);
    const std::size_t n = std::min<std::size_t>(dest.COUNT, j.size());
    for(std::size_t i = 0; i < n; ++i){
        dest.values[i] = j[i].get<int>();
    }
    return dest;
}
