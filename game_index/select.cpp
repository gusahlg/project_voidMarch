#include "raylib.h"
#include <iostream>
#include "main_select.h"
class selectScr{
   public:
   void selectScrUpdate(){
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText("GAME SETUP", 540, 20, 75, BLACK);
        DrawText("FPS:", 1100, 20, 20, BLACK);
        DrawFPS(1150, 20);
        EndDrawing();
   }
   class Sbuttons{
       
   };
};