#include <raylib.h>
#include <iostream>
#include <cstdint>
// Provides standard ui to other files:
#include "include/standardUI/globalC.hpp"
// Provides gameloop functions to int main:
#include "include/game/loadGame.hpp"
// Select button output:
#include "include/screen_load/select_main.hpp"
Vector2 mousePos;
Color Ctext = {160, 160, 160, 255};
Color spaceBlue = {25, 70, 240, 255};
// Use to determine what to load:
enum screen : std::uint8_t {Main, Select, Game};
screen currentScreen;
int main(){
    InitWindow(1280, 720, "PROJECT: VOIDMARCH");
    Image icon = LoadImage("assets/graphics/logos/VoidMarchLogo.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    // Loop that the entire game runs within
    while(!WindowShouldClose()){
        mousePos = GetMousePosition();
        BeginDrawing();
        switch(currentScreen){
            case screen::Main: // Do da load main stuff.
            case screen::Select: // Do da load select stuff.
            case screen::Game: gameStateEventHandler();
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
/*if(IsKeyPressed(KEY_F11)){
            ToggleFullscreen();
        }*/