#pragma once
#include "../json_helpers/data_communicator.hpp"
#include <utility>
#include <raylib.h>
#include <cstdint>
#include <string_view>
class Player {
public:
    void takeDamage(int d){
        data.set(info::Hp, data.get(info::Hp)-d);
    }
    void setHp(int newValue){
        data.set(Hp, newValue);
    }
    void setInt(int newValue){
        data.set(Int, newValue);
    }
    void setCoolness(int newValue){
        data.set(Coolness, newValue);
    }
    void setCoords(Vector2 newCoords){
        float x = floorf(newCoords.x);
        float y = floorf(newCoords.y);
        data.set(CordX, x);
        data.set(CordY, y);
    }
    bool isDead() const{
        return(data.get(info::Hp) <= 0);
    }
    int HP() const{
        return data.get(info::Hp);
    }
    int INT() const{
        return data.get(info::Int);
    }
    int COOLNESS() const{
        return data.get(info::Coolness);
    }
    Vector2 Coords(){
        return {(float)data.get(info::CordX), (float)data.get(info::CordY)};
    }
    Player()
    : data{} {
        data = createStats("game_index/save/player.json");
    }
private:
    enum info : std::uint8_t{Hp, Int, Coolness, CordX, CordY};
    Stats data;
};
//Gives easy access to stats.