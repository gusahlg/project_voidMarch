//paths work
#include <raylib.h>
#include <iostream>
#include "include/standardUI/globalC.hpp"
#include "include/select/main_select.h"
#include "include/select/Sbuttons.hpp"
#include "include/game/loadGame.hpp"
Color Ctext = {160, 160, 160, 255};
Color gRed = {255, 20, 10, 255};
Color spaceBlue = {25, 70, 240, 255};
Color selectC = {5, 2, 33, 255};
Color btnP = {3, 8, 54, 255};
Color btnH = {14, 6, 92, 255};
Color btnC = {27, 6, 107, 255};
Rectangle button = {540, 300, 200, 75};
Sbuttons Btn1{1, {300, 260, 150, 50}};
Sbuttons Btn2{2, {300, 340, 150, 50}};
Sbuttons Btn3{3, {300, 420, 150, 50}};
void updateHome();
void updateSelect();
void updateGame();
void screen::update(){
    switch(screenIdentifier){
        case(1):
            updateHome();
            break;
        case(2):
            updateSelect(); 
            break;
        case(3):
            updateGame();
            break;
    }
}
bool Hclicked = false;
int main(){
    InitWindow(1280, 720, "PROJECT: VOIDMARCH");
    SetTargetFPS(100);
    screen home; home.screenIdentifier = 1;
    screen select; select.screenIdentifier = 2;
    screen game; game.screenIdentifier = 3;
    while(!WindowShouldClose()){
        mousePos = GetMousePosition();
        if(IsKeyPressed(KEY_F11)){
            ToggleFullscreen();
        }
        BeginDrawing();
        if(!Hclicked){
            ClearBackground(spaceBlue);
            home.update();
        }
        //The game is running
        else if(gameRunning){
            loadLvl1();
        }
        else if(Hclicked){
            ClearBackground(selectC);
            panel.selectScrUpdate();
            Btn1.update();
            Btn2.update();
            Btn3.update();
            select.update();
            checkForBtnState();
            setupChoiceEventHandler();
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
float btnroundness = 0.65f;
int segs = 20;
Vector2 mousePos = GetMousePosition();
void updateHome(){
    bool hover  = CheckCollisionPointRec(mousePos, button);
    bool pressed = hover && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool clicked = hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    if(clicked){
        Hclicked = true;
    }
    Color Cstate = pressed ? btnP : hover ? btnH : btnC;
    DrawText("PROJECT VOIDMARCH", 20, 20, 75, BLACK);
    DrawText("FPS:", 1100, 20, 20, gRed);
    DrawFPS(1150, 20);
    DrawRectangleRounded(button, btnroundness, segs, Cstate);
    DrawRectangleRoundedLines(button, btnroundness, segs, PINK);
    DrawText("SETUP GAME", 545, 320, 28, gRed);
}
void updateSelect(){
    DrawText("GAME SETUP", 400, 90, 75, PINK);
}
void updateGame(){
    ClearBackground(YELLOW);
    DrawText("PROJECT VOIDMARCH", 20, 20, 75, BLACK);
    DrawText("FPS:", 1100, 20, 20, gRed);
    DrawFPS(1150, 20);
}
