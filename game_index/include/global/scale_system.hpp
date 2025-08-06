#pragma once
#include "scale_utils.hpp"
#include "raylib.h"

class ScaleSystem{
public:
    const ScaleInfo& info() const{return si;}
    template<typename LevelT>
    void update(const LevelT& lvl){
        si = makeScaleInfo(GetScreenWidth(), GetScreenHeight(), lvl.rows[0].size(), lvl.rows.size());
    }
private:
    ScaleInfo si{};
};
extern ScaleSystem scaleSys;