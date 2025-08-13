#include "raylib.h"
#include <iostream>
#include <cstdint>
#include <vector>
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
enum class screen : std::uint8_t {Main, Select, Game};
screen currentScreen = screen::Game;
int screenWidth = 1280;
int screenHeight = 720;
int main(){
    InitWindow(screenWidth, screenHeight, "PROJECT: VOIDMARCH");
    SetTargetFPS(60);
    Image icon = LoadImage("assets/graphics/logos/VoidMarchLogo.png");
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
                // Do da load main stuff.
                break;
            case screen::Select:
                loadSelectScreen(mousePos, screenWidth, screenHeight);
                break;
            case screen::Game: 
                gameStateEventHandler();
                break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}