#include "raylib.h"
#include <iostream>
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
struct ButtonStyle{
    Color bg[4];
    Color fg[4];
    Font  font;
    float radius = 8; 
    float padding = 12;
    Texture2D image = {0};
    Rectangle srcRect = {0}; 
};
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
        }
    }
    CloseWindow();
    return 0;
}
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
    DrawRectangleRec(button, Cstate);
    DrawText("SETUP GAME", 545, 320, 28, gRed);
    EndDrawing();
}
void updateSelect(){
    BeginDrawing();
    ClearBackground(selectC);
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
