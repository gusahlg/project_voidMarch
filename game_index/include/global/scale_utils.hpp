#pragma once
#include <algorithm>
#include <iostream>
#include <cmath>
#include "raylib.h"
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
inline ScaleInfo makeScaleInfo(int screenW, int screenH, int lvlW, int lvlH, int zoomMul){
    float base = computeScaleFromHeight(screenH, lvlH);           // integer-ish base, >=1
    float s    = base * std::max(1, zoomMul);                     // user zoom on top
    float tilePx = TILE_SIZE * s;
    int visX   = static_cast<int>(std::ceil(screenW / tilePx));
    return {s, tilePx, visX};
}
inline float toPx(float tiles, const ScaleInfo& si){
    return tiles * si.tilePx;
}
inline float toTiles(float px, const ScaleInfo& si){
    return px / si.tilePx;
}
//Snaps floats to closest int, used for drawing stuff on screen.
inline void Snap(Rectangle& r){
    r.x = floorf(r.x + 0.5f);
    r.y = floorf(r.y + 0.5f);
    r.width = floorf(r.width + 0.5f);
    r.height = floorf(r.height + 0.5f);
}