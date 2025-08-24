#include <raylib.h>
#include <iostream>
#include "../include/game/player_stats.hpp"
#include "../include/screen_load/screen_constants.hpp"
#include <cstdint>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <random>
// Can communicate with main.cpp:
#include "../include/screen_load/select_main.hpp"
// Gives easy access to all standard ui features:
#include "../include/standardUI/manager.hpp"
// The files' local ui manager:
ui::Manager mainManager;
void levelSelectPreLoadTasks(float screenWidth, float screenHeight){
    static Texture2D Idle = LoadTexture("assets/ui/screen_interface/buttons/Idle.png");
    static Texture2D Hover = LoadTexture("assets/ui/screen_interface/buttons/Hover.png");
    static Texture2D Pressed = LoadTexture("assets/ui/screen_interface/buttons/Pressed.png");
    const float btnX = screenWidth/2.f-Idle.width/2.f;
    const float btnY = screenHeight/2.f-Idle.height/2.f;
    mainManager.emplaceButton(Rectangle{btnX,btnY,float(Idle.width),float(Idle.height)},Idle,Hover,Pressed,[]{currentScreen=screen::Select;});
}
void loadLevelSelect(Vector2 mousePos, float screenWidth, float screenHeight){
    static bool loaded = false;
    if(!loaded){
        levelSelectPreLoadTasks(screenWidth, screenHeight);
        loaded = true;
    }
    mainManager.updateAll(mousePos);
    mainManager.drawAll();
}

