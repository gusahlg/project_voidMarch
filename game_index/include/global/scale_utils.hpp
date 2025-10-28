#pragma once
#include "constants.hpp"
namespace ScaleUtils{

inline float toPx(int tiles){
    return static_cast<float>(tiles*TILE_SIZE);
}
inline int toTiles(int pixels){
    return static_cast<float>(pixels/TILE_SIZE);
}

}