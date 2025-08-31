#pragma once
#include "../json_helpers/data_communicator.hpp"
#include <utility>
#include <cstdint>
#include <raylib.h>
#include <string_view>
class World{
public:
    void saveWorldData(int playerX, int playerY, int currentLevel){
        data.set(info::PlayerX, playerX);
        data.set(info::PlayerY, playerY);
        data.set(info::CurrentLevel, currentLevel);
        data.save();
    }
    Vector2 playerPos() const{
        Vector2 value = {(float)data.get(info::PlayerX), (float)data.get(info::PlayerY)};
        return value;
    }
    int currentLevel() const{
        return data.get(info::CurrentLevel);
    }
    World()
    : data{}{
        data = createStats("game_index/save/world.json");
    }
private:
    enum info : std::uint8_t{PlayerX, PlayerY, CurrentLevel};
    Stats data;
};
//Gives easy access to world data.