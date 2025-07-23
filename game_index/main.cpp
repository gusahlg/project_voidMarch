#include <raylib.h>
#include <iostream>
#include "include/standardUI/globalC.hpp"
#include "include/select/Sbuttons.hpp"
#include "include/game/loadGame.hpp"
Vector2 mousePos = GetMousePosition();
Color Ctext = {160, 160, 160, 255};
Color spaceBlue = {25, 70, 240, 255};
int main(){
    InitWindow(1280, 720, "PROJECT: VOIDMARCH");
    Image icon = LoadImage("assets/graphics/logos/VoidMarchLogo.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    // Loop that the entire game runs within
    while(!WindowShouldClose()){
        mousePos = GetMousePosition();
        BeginDrawing();
        loadLvl1();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
/*if(IsKeyPressed(KEY_F11)){
            ToggleFullscreen();
        }*/