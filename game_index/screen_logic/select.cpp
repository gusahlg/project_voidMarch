#include <raylib.h>
#include <iostream>
#include "../include/game/player_stats.hpp"
#include "../include/screen_load/screen_constants.hpp"
#include <cstdint>
#include <vector>
#include <cmath>
#include "../include/screen_load/home_screen.hpp"
// Can communicate with main.cpp:
#include "../include/screen_load/select_main.hpp"
// Gives easy access to all standard ui features:
#include "../include/standardUI/manager.hpp"
// The files' local ui manager:
ui::Manager selectManager;
using DP = ui::Button::DisplayDependency;
Race currentRace;
Prof currentProf;
bool raceSelect=false;
bool profSelect=false;
void selectPreLoadTasks(float screenWidth, float screenHeight){
    static Texture2D Idle = LoadTexture("assets/ui/screen_interface/buttons/Idle.png");
    static Texture2D Hover = LoadTexture("assets/ui/screen_interface/buttons/Hover.png");
    static Texture2D Pressed = LoadTexture("assets/ui/screen_interface/buttons/Pressed.png");
    // Race select buttons
    selectManager.emplaceButton(Rectangle{0, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Spacelizard;raceSelect=true;});
    selectManager.emplaceButton(Rectangle{50, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Voidcrawler;raceSelect=true;});
    selectManager.emplaceButton(Rectangle{100, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Mecha_sapien;raceSelect=true;});
    selectManager.emplaceButton(Rectangle{150, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Human;raceSelect=true;});
    // Proffesion select buttons
    selectManager.emplaceButton(Rectangle{0, 50, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentProf = Prof::Necromancer;profSelect=true;});
    selectManager.emplaceButton(Rectangle{50, 50, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentProf = Prof::Craftsman;profSelect=true;});
    selectManager.emplaceButton(Rectangle{100, 50, float(Idle.width), float(Idle.height)}, Idle, Hover, Pressed,[]{currentProf = Prof::Wizard;profSelect=true;});
    selectManager.emplaceButton(Rectangle{150, 50, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentProf = Prof::Dualist;profSelect=true;});
    // Create background:
    selectManager.setupHelper(Idle);
    selectManager.Helper.setupScale(screenWidth, screenHeight);
}
void loadSelectScreen(Vector2 mousePos, float screenWidth, float screenHeight){
    static bool loaded = false;
    if(!loaded){
        selectPreLoadTasks(screenWidth, screenHeight);
        loaded = true;
    }
    selectManager.updateAll(mousePos);
    selectManager.drawAll();
    selectManager.Helper.draw();
    if(raceSelect && profSelect) currentScreen = screen::Game;
}
