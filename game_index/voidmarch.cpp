#include "raylib.h"
Color gRed = {255, 20, 10, 255};
Color spaceBlue = {20, 50, 240, 255};

int main(){
    InitWindow(1500, 900, "Coolhetsstället");
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(spaceBlue);
        DrawText("Gutentag!", 20, 20, 100, gRed);
        DrawText("FPS:", 1240, 16, 25, gRed);
        DrawFPS(1300, 20);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
