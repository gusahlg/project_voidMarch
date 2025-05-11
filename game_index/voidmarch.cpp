#include "raylib.h"

int main() {
    InitWindow(800, 450, "Gustav’s raylib test");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hej världen!  FPS:", 20, 20, 20, DARKGRAY);
        DrawFPS(160, 20);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
