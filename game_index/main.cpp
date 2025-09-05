#include "raylib.h"
#include <iostream>
#include "include/game/player_stats.hpp"
#include "include/screen_load/home_screen.hpp"
// Provides access to screen enum for other files:
#include "include/screen_load/screen_constants.hpp"
// Provides standard ui to other files:
#include "include/standardUI/globalC.hpp"
// Provides gameloop functions to int main:
#include "include/game/loadGame.hpp"
// Select button output:
#include "include/screen_load/select_main.hpp"
// Gives easy access to all standard ui features:
#include "include/standardUI/manager.hpp"
// RUSTLANG INTEGRATION LIBRARY:
#include "math_core.h"
Vector2 mousePos;
Color Ctext = {160, 160, 160, 255};
Color spaceBlue = {25, 70, 240, 255};
// Use to determine what to load:
screen currentScreen = screen::Main;
int screenWidth = 1280;
int screenHeight = 720;
int main(){
    InitWindow(GetScreenWidth(), GetScreenHeight(), "PROJECT: VOIDMARCH");
    SetWindowState(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(60);
    static Image icon = LoadImage("assets/graphics/logos/VoidMarchLogo.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    // Loop that the entire game runs within
    while(!WindowShouldClose()){
        mousePos = GetMousePosition();
        BeginDrawing();
        ClearBackground(BLACK);
        if(IsKeyPressed(KEY_F11)){
            ToggleFullscreen();
        }
        switch(currentScreen){
            case screen::Main: 
                loadMainScreen(mousePos, screenWidth, screenHeight);
                break;
            case screen::Select:
                loadSelectScreen(mousePos, screenWidth, screenHeight);
                break;
            case screen::Game: 
                gameStateEventHandler();
                break;
            case screen::LevelSelect:
                loadLevelSelect(mousePos, screenWidth, screenHeight);
                break;
        }
        EndDrawing();
    }
    PlayerTexManager::instance().unloadAll();
    CloseWindow();
    return 0;
}