#pragma once
#include "../json_helpers/data_communicator.hpp"
#include <utility>
class Player {
public:
    void takeDamage(int d){
        stats_.damage(d);
    }
    bool isDead() const{
        return stats_.isDead();
    }
    int hp() const{
        return stats_.get(Stat::HP); 
    }
    int mp() const{
        return stats_.get(Stat::MP); 
    }
    int str() const{
        return stats_.get(Stat::STR); 
    }
    const Stats& raw() const{
        return stats_;
    }
    Player(Stats s)
    : stats_(std::move(s)) {}
private:
    Stats stats_;
};
//Gives easy access to stats.