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
ui::Manager selectManager;
using DP = ui::Button::DisplayDependency;
Race currentRace;
Prof currentProf;
void selectPreLoadTasks(float screenWidth, float screenHeight){
    static Texture2D Idle = LoadTexture("assets/ui/screen_interface/buttons/Idle.png");
    static Texture2D Hover = LoadTexture("assets/ui/screen_interface/buttons/Hover.png");
    static Texture2D Pressed = LoadTexture("assets/ui/screen_interface/buttons/Pressed.png");
    // Race select buttons
    selectManager.emplaceButton(Rectangle{0, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Spacelizard;}, "Bert");
    selectManager.emplaceButton(Rectangle{50, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Voidcrawler;}, "Sighurt", DP::OnHover, "Bert");
    selectManager.emplaceButton(Rectangle{100, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Mecha_sapien;}, "Gertrud", DP::OnClick, "Sighurt");
    selectManager.emplaceButton(Rectangle{150, 0, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentRace = Race::Human;});
    // Proffesion select buttons
    selectManager.emplaceButton(Rectangle{0, 50, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentProf = Prof::Necromancer;});
    selectManager.emplaceButton(Rectangle{50, 50, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentProf = Prof::Craftsman;});
    selectManager.emplaceButton(Rectangle{100, 50, float(Idle.width), float(Idle.height)}, Idle, Hover, Pressed,[]{currentProf = Prof::Wizard;});
    selectManager.emplaceButton(Rectangle{150, 50, float(Idle.width), float(Idle.height)},Idle, Hover, Pressed,[]{currentProf = Prof::Dualist;});
    // Create background:
    selectManager.setupHelper(Idle, []{DrawRectangle(100, 50, 500, 500, YELLOW);});
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
    selectManager.Helper.updateFuncs();
    selectManager.Helper.draw();
}
