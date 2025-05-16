#include "raylib.h"
#include <iostream>
#include "main_select.h"
Color gRed = {255, 20, 10, 255};
Color spaceBlue = {25, 70, 240, 255};
Color selectC = {5, 2, 33, 255};
Color btnP = {3, 8, 54, 255};
Color btnH = {14, 6, 92, 255};
Color btnC = {27, 6, 107, 255};
Rectangle button = {540, 300, 200, 75};
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
        if(!Hclicked){
            home.update();
        }
        else if(Hclicked){
            select.update();
            panel.selectScrUpdate();
        }
    }
    CloseWindow();
    return 0;
}
float btnroundness = 0.65f;
int segs = 20;
void updateHome(){
    Vector2 mousePos = GetMousePosition();
    bool hover  = CheckCollisionPointRec(mousePos, button);
    bool pressed = hover && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool clicked = hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    if(clicked){
        Hclicked = true;
    }
    Color Cstate = pressed ? btnP : hover ? btnH : btnC;
    BeginDrawing();
    ClearBackground(spaceBlue);
    DrawText("PROJECT VOIDMARCH", 20, 20, 75, BLACK);
    DrawText("FPS:", 1100, 20, 20, gRed);
    DrawFPS(1150, 20);
    DrawRectangleRounded(button, btnroundness, segs, Cstate);
    DrawRectangleRoundedLines(button, btnroundness, segs, PINK);
    DrawText("SETUP GAME", 545, 320, 28, gRed);
    EndDrawing();
}
void updateSelect(){
    BeginDrawing();
    ClearBackground(selectC);
    DrawText("GAME SETUP", 540, 20, 75, BLACK);
    DrawText("FPS:", 1100, 20, 20, gRed);
    DrawFPS(1150, 20);
    EndDrawing();
}
void updateGame(){
    BeginDrawing();
    ClearBackground(YELLOW);
    DrawText("PROJECT VOIDMARCH", 20, 20, 75, BLACK);
    DrawText("FPS:", 1100, 20, 20, gRed);
    DrawFPS(1150, 20);
    EndDrawing();
}
