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
    bool isDead(){
        if(data.get(info::Hp) <= 0) return true;
        else return false;
    }
    int HP(){
        return data.get(info::Hp);
    }
    int INT(){
        return data.get(info::Int);
    }
    int COOLNESS(){
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