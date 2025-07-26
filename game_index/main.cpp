#include <raylib.h>
#include <iostream>
#include <cstdint>
#include <vector>
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
enum class screen : std::uint8_t {Main, Select, Game};
screen currentScreen = screen::Select;
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
            case screen::Main: 
                // Do da load main stuff.
                break;
            case screen::Select:
                static bool loaded = false;
                if(!loaded){
                    std::vector<Rectangle> btns;
                    // Make da buttons
                    Texture tex; // Add an actual texture at some point
                    btns.emplace_back(Rectangle{0, 0, 100, 100});
                    btns.emplace_back(Rectangle{100, 0, 100, 100});
                    btns.emplace_back(Rectangle{200, 0, 100, 100});
                    btns.emplace_back(Rectangle{300, 0, 100, 100});
                    BtnAssembly(btns, tex);
                    loaded = true;
                }
                updateButtons(mousePos);
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
/*if(IsKeyPressed(KEY_F11)){
            ToggleFullscreen();
        }*/