#include <raylib.h>
#include <iostream>
#include "../include/game/player_stats.hpp"
#include <cstdint>
#include <vector>
#include <cmath>
bool raceSPACELIZARD = false;
bool raceVOIDCRAWLER = false;
bool raceMECHA_SAPIEN = false;
bool raceHUMAN = true;

bool profNECROMANCER = true;
bool profCRAFTSMAN = false;
bool profWIZARD = false;
bool profDUALIST = false;

struct button{
    Rectangle rect;
    enum state : std::uint8_t{Inactive, Hover, Pressed, Clicked};
    void determineState(Vector2 mousePos){
        bool inside = mousePos.x - rect.x < rect.width && mousePos.y - rect.y < rect.height;
        if(!inside) return;
        

    }
};
std::vector<button> buttons;
void updateButtons(){

}