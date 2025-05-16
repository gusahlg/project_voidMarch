#include "raylib.h"
#include <iostream>
#include "main_select.h"
void selectScr::selectScrUpdate(){
   Rectangle selectPanel{128, 36, 1024, 648};
   BeginDrawing();
   DrawRectangleRounded(selectPanel, 0.3, 20, DARKGRAY);
   ClearBackground(selectC);
   EndDrawing();
}
class Sbuttons{
   public:
      Rectangle button;    
};
selectScr panel;