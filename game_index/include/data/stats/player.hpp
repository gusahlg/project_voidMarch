#pragma once
#include "../json_helpers/data_communicator.hpp"
#include <utility>
#include <string_view>
class Player {
public:
    enum info{HP, INT, COOLNESS};
    void takeDamage(int d){
        data.set(info::HP, (int)data.get(info::HP)-d);
    }
    bool isDead(){
        if(data.get(info::HP) <= 0) return true;
        else return false;
    }
    int HP(){
        return data.get(info::HP);
    }
    int INT(){
        return data.get(info::INT);
    }
    int COOLNESS(){
        return data.get(info::COOLNESS);
    }
    Player()
    : data{} {
        data = createStats("game_index/save/player.json");
    }
private:
    Stats data;
};
//Gives easy access to stats.