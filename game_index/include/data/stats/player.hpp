#pragma once
#include "player_data.hpp"
#include <utility>
class Player {
public:
    explicit Player(Stats s) : stats_(std::move(s)) {}
    void takeDamage(int d){ stats_.damage(d); }
    bool isDead()     const{ return stats_.isDead(); }
    int  hp()   const{ return stats_.get(Stat::HP); }
    int  mp()   const{ return stats_.get(Stat::MP); }
    int  str()  const{ return stats_.get(Stat::STR); }
    const Stats& raw() const { return stats_; }
private:
    Stats stats_;
};