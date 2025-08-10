#pragma once
#include <algorithm>
#include <iostream>
#include <cmath>
#include "constants.hpp"
inline float computeScaleFromHeight(int screenH, int levelHeightTiles){
    float s = static_cast<float>(screenH)/(levelHeightTiles*TILE_SIZE);
    s = std::max(1.0f, floorf(s));
    return (int)s;
}
struct ScaleInfo{
    float scale;
    float tilePx;
    int visTilesX;
};
inline ScaleInfo makeScaleInfo(int screenW, int screenH, int lvlW, int lvlH){
    float s = computeScaleFromHeight(screenH, lvlH);
    float tilePx = TILE_SIZE * s;
    int visX = static_cast<int>(std::ceil(screenW/tilePx));
    return {s, tilePx, visX};
}
inline float toPx(float tiles, const ScaleInfo& si){
    return tiles * si.tilePx;
}
inline float toTiles(float px, const ScaleInfo& si){
    return px / si.tilePx;
}