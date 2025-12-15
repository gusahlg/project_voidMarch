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
    static Texture2D button = LoadTexture("assets/UI/screen_interface/buttons/blueButton.png");
    // Race select buttons
    selectManager.emplaceButton(Vector2{0, 0},button,[]{currentRace = Race::Spacelizard;raceSelect=true;});
    selectManager.emplaceButton(Vector2{50, 0},button,[]{currentRace = Race::Voidcrawler;raceSelect=true;});
    selectManager.emplaceButton(Vector2{100, 0},button,[]{currentRace = Race::Mecha_sapien;raceSelect=true;});
    selectManager.emplaceButton(Vector2{150, 0},button,[]{currentRace = Race::Human;raceSelect=true;});
    // Proffesion select buttons
    selectManager.emplaceButton(Vector2{0, 50},button,[]{currentProf = Prof::Necromancer;profSelect=true;});
    selectManager.emplaceButton(Vector2{50, 50},button,[]{currentProf = Prof::Craftsman;profSelect=true;});
    selectManager.emplaceButton(Vector2{100, 50},button,[]{currentProf = Prof::Wizard;profSelect=true;});
    selectManager.emplaceButton(Vector2{150, 50},button,[]{currentProf = Prof::Dualist;profSelect=true;});

    // Button progresses to level select
    const float btnX = screenWidth/2.f;
    const float btnY = screenHeight/2.f;
    selectManager.emplaceButton(Vector2{btnX, btnY},button,[]{if(raceSelect && profSelect) currentScreen = screen::LevelSelect;});
    // Create background:
    selectManager.setupHelper(button);
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
}
