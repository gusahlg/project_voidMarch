#include "raylib.h"
#include <iostream>
#include "main_select.h"
void selectScr::selectScrUpdate(){
   Rectangle selectPanel{128, 80, 1024, 600};
   DrawRectangleRounded(selectPanel, 0.2, 100, DARKGRAY);
}
class Sbuttons{
   public:
      Rectangle button;    
};
selectScr panel;