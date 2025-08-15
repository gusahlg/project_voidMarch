#pragma once
#include "scale_utils.hpp"
#include "raylib.h"

class ScaleSystem{
public:
    const ScaleInfo& info() const { return si; }

    int zoom() const { return zoomMul; }
    void setZoom(int z){ zoomMul = std::clamp(z, MIN_ZOOM, MAX_ZOOM); }
    void nudgeZoom(int dz){ setZoom(zoomMul + dz); }

    template<typename LevelT>
    void update(const LevelT& lvl){
        si = makeScaleInfo(GetScreenWidth(), GetScreenHeight(),
                           (int)lvl.rows[0].size(), (int)lvl.rows.size(),
                           zoomMul);
    }
private:
    static constexpr int MIN_ZOOM = 1;
    static constexpr int MAX_ZOOM = 4; // adjust if you want more
    int zoomMul = 1;
    ScaleInfo si{};
};