#include <raylib.h>
#include <iostream>
#include "../include/game/player_stats.hpp"
#include <cstdint>
#include <vector>
#include <cmath>
// Can communicate with main.cpp:
#include "../include/screen_load/select_main.hpp"
// Gives easy access to all standard ui features:
#include "../include/standardUI/manager.hpp"
// The files local ui manager:
ui::Manager selectBtnManager;
bool raceSPACELIZARD = false;
bool raceVOIDCRAWLER = false;
bool raceMECHA_SAPIEN = false;
bool raceHUMAN = true;

bool profNECROMANCER = true;
bool profCRAFTSMAN = false;
bool profWIZARD = false;
bool profDUALIST = false;
enum class prof : std::uint8_t{Necromancer, Craftsman, Wizard, Dualist};
prof currentProf;
enum class race : std::uint8_t{Spacelizard, Voidcrawler, Mecha_sapien, Human};
race currentRac; // Can't be named 'currentRace' since that is the same as another enum in game.cpp, will fix at some point.
void selectPreLoadTasks(){
    static Texture2D Idle = LoadTexture("assets/ui/screen_interface/buttons/Idle.png");
    static Texture2D Hover = LoadTexture("assets/ui/screen_interface/buttons/Hover.png");
    static Texture2D Pressed = LoadTexture("assets/ui/screen_interface/buttons/Pressed.png");
    selectBtnManager.emplaceButton(Rectangle{0, 0, float(Idle.width), float(Idle.height)},
                                   Idle, Hover, Pressed,
                                   []{ToggleFullscreen();});
}
void loadSelectScreen(Vector2 mousePos){
    // Update and initialise buttons here please.
    static bool loaded = false;
    if(!loaded){
        selectPreLoadTasks();
        loaded = true;
    }
    selectBtnManager.updateAll(mousePos);
    selectBtnManager.drawAll();
}