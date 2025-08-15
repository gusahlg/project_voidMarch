#pragma once
#include <array>
#include <cstdint>
#include <algorithm>
#include "load_save.hpp"
#include <string_view>
#include <vector>
#include "../nlohmann/json.hpp"
// This is a full data handler API that speaks with json files and stores values.
// What you want to set and what you want to set it to
struct Stats{
    void set(const int& valueType, const int& newValue){
        values[valueType] = newValue;
        save_json(file, *this);
    }
    int get(const int& valueType) const{
        return values[valueType];
    }
    void bind_file(std::string_view f){
        file = f;
    }
    std::string_view file{};
    std::vector<int> values{};
    std::size_t COUNT{};
    Stats() 
    : COUNT{}, values{}, file{} {}
};
inline Stats createStats(std::string_view file){
    Stats s;
    s.bind_file(file);
    auto tmp = load_json<Stats>(file);
    s.values = std::move(tmp.values);
    s.COUNT = tmp.COUNT;
    return s;
}