#include "../math/ahl_math.hpp"
#include "../../include/game/enemy_data.hpp"
namespace Combat{
inline void enemyCircleSectorCollHandler(int range, Vector2 playerPos, int damage){
    for(auto& e : enemies){
        if(!AhlMath::sectorColl(range, playerPos, e.Hbox)) continue;
        e.HP -= damage;
    }
}
}