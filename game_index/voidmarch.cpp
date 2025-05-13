#include "raylib.h"
#include <iostream>
Color gRed = {255, 20, 10, 255};
Color spaceBlue = {25, 70, 240, 255};
void updateHome();
void updateSelect();
void updateGame();
class screen{
    public:
        int screenIdentifier;
    void update(){
        switch(screenIdentifier){
            case(1):
                updateHome();
                break;
            case(2):
                updateSelect(); 
                break;
            case(3):
                updateGame();
        }
    }
};
int main(){
    InitWindow(1280, 720, "PROJECT: VOIDMARCH");
    screen home; home.screenIdentifier = 1;
    screen select; select.screenIdentifier = 2;
    screen game; game.screenIdentifier = 3;
    while(!WindowShouldClose()){
        home.update();
    }
    CloseWindow();
    return 0;
}
void updateHome(){
    BeginDrawing();
    ClearBackground(spaceBlue);
    DrawText("PROJECT VOIDMARCH", 20, 20, 75, BLACK);
    DrawText("FPS:", 1100, 20, 20, gRed);
    DrawFPS(1150, 20);
    EndDrawing();
}
void updateSelect(){
    BeginDrawing();
    ClearBackground(GREEN);
    DrawText("PROJECT VOIDMARCH", 20, 20, 75, BLACK);
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
