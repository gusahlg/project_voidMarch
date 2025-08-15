#pragma once
#include "../json_helpers/data_communicator.hpp"
#include <utility>
#include <string_view>
class Player {
public:
    enum info{Hp, Int, Coolness};
    void takeDamage(int d){
        data.set(info::Hp, (int)data.get(info::Hp)-d);
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
    Player()
    : data{} {
        data = createStats("game_index/save/player.json");
    }
private:
    Stats data;
};
//Gives easy access to stats.