#pragma once
#include "raylib.h"
#include "raymath.h"
#include <cmath>
namespace AhlMath{

inline bool sectorColl(int radiusPx, Vector2 origin, Rectangle targetRect){
    Vector2 distanceVec = {origin.x - targetRect.x-(targetRect.width/2), origin.y - targetRect.y-(targetRect.height/2)};
    float distance = sqrtf((distanceVec.x*distanceVec.x) + (distanceVec.y*distanceVec.y));
    float targetRadius = targetRect.width;
    float CombinedRadius = targetRadius + radiusPx;
    return CombinedRadius > distance && CombinedRadius < distance*2.f;
}


}
