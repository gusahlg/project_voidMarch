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
// The files' local ui manager:
ui::Manager selectBtnManager;
Race currentRace;
Prof currentProf;
void selectPreLoadTasks(){
    static Texture2D Idle = LoadTexture("assets/ui/screen_interface/buttons/Idle.png");
    static Texture2D Hover = LoadTexture("assets/ui/screen_interface/buttons/Hover.png");
    static Texture2D Pressed = LoadTexture("assets/ui/screen_interface/buttons/Pressed.png");
    // Race select buttons
    selectBtnManager.emplaceButton(Rectangle{0, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Spacelizard;});
    selectBtnManager.emplaceButton(Rectangle{50, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Voidcrawler;});
    selectBtnManager.emplaceButton(Rectangle{100, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Mecha_sapien;});
    selectBtnManager.emplaceButton(Rectangle{150, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Human;});
    // Proffesion select buttons
    selectBtnManager.emplaceButton(Rectangle{0, 50, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentProf = Prof::Necromancer;});
    selectBtnManager.emplaceButton(Rectangle{50, 50, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentProf = Prof::Craftsman;});
    selectBtnManager.emplaceButton(Rectangle{100, 50, float(Idle.width), float(Idle.height)}, Idle, Hover, Pressed,[]{currentProf = Prof::Wizard;});
    selectBtnManager.emplaceButton(Rectangle{150, 50, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentProf = Prof::Dualist;});
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