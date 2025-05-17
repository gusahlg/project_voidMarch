#include "raylib.h"
#include <iostream>
#include "main_select.h"
#include "Sbuttons.h"
void selectScr::selectScrUpdate(){
   Rectangle selectPanel{128, 80, 1024, 600};
   DrawRectangleRounded(selectPanel, 0.2, 100, DARKGRAY);
}
selectScr panel;