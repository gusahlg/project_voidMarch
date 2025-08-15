#pragma once
#include <array>
#include <cstdint>
#include <algorithm>
#include "load_save.hpp"
#include <string_view>
#include <vector>
#include "../nlohmann/json.hpp"
// This is a full data handler API that speaks with json files and stores values.
std::size_t value_count_reader(std::string_view path){
    std::ifstream data{std::string(path)};
    nlohmann::json j;
    data >> j;
    return j.size();
}
// What you want to set and what you want to set it to
class Stats{
public:
    void set(const int& valueType, const int& newValue){
        values[valueType] = newValue;
    }
    int get(const int& valueType){
        return values[valueType];
    }
    std::vector<int> values{};
    std::size_t COUNT{};
    Stats() 
    : COUNT{}, values{}, file{} {}
private:
    std::string_view file{};
};
Stats createStats(std::string_view file){
    Stats s;
    s.COUNT = value_count_reader(file);
    s = load_json(file);
    return std::move(s);
}