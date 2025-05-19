#include "raylib.h"
#include <iostream>
#include "main_select.h"
#include "Sbuttons.hpp"
void selectScr::selectScrUpdate(){
   Rectangle selectPanel{128, 80, 1024, 600};
   DrawRectangleRounded(selectPanel, 0.2, 100, DARKGRAY);
}
bool optHover1;
bool optHover2;
bool optHover3;
bool hover1 = CheckCollisionPointRec(mousePos, Btn1.button);
bool hover2 = CheckCollisionPointRec(mousePos, Btn2.button);
bool hover3 = CheckCollisionPointRec(mousePos, Btn3.button);
bool pressed1 = hover1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
bool clicked1 = hover1 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
bool pressed2 = hover2 && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
bool clicked2 = hover2 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
bool pressed3 = hover3 && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
bool clicked3 = hover3 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
selectScr panel;
void checkForBtnState(){
   hover1  = CheckCollisionPointRec(mousePos, Btn1.button);
   hover2  = CheckCollisionPointRec(mousePos, Btn2.button);
   hover3  = CheckCollisionPointRec(mousePos, Btn3.button);
   pressed1 = hover1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
   clicked1 = hover1 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   pressed2 = hover2 && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
   clicked2 = hover2 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   pressed3 = hover3 && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
   clicked3 = hover3 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}
Color Chover = {143, 44, 113, 255};
Color Cpressed = {92, 21, 70, 255};
Color CB = {156, 44, 142, 255};
void Ddifficulties(){
   Color optBackground = {15, 15, 15, 175};
   Rectangle options {450, 260, 150, 200};
   DrawRectangleRounded(options, 0.1, 10, optBackground);
   optHover1 = CheckCollisionPointRec(mousePos, options);
   Rectangle EASY {450, 260, 150, 50};
   Rectangle MEDIUM {450, 310, 150, 50};
   Rectangle HARD {450, 360, 150, 50};
   Rectangle PRO_GAMER {450, 410, 150, 50};
   
   DrawText("EASY", 1, 1, 1, BLACK);
   DrawText("MEDIUM", 1, 1, 1, BLACK);
   DrawText("HARD", 1, 1, 1, BLACK);
   DrawText("PRO GAMER", 1, 1, 1, BLACK);
}
void Drace(){
   Color optBackground = {15, 15, 15, 175};
   Rectangle options{450, 340, 150, 200};
   DrawRectangleRounded(options, 0.1, 10, optBackground);
   Rectangle HUMAN {450, 340, 150, 50};
   Rectangle SPACELIZARD {450, 390, 150, 50};
   Rectangle VOIDCRAWLER {450, 440, 150, 50};
   Rectangle MECHA_SAPIEN {450, 490, 150, 50};

   DrawRectangleRounded(HUMAN, 0.1, 10, PINK);
   DrawRectangleRounded(SPACELIZARD, 0.1, 10, RED);
   DrawRectangleRounded(VOIDCRAWLER, 0.1, 10, BLUE);
   DrawRectangleRounded(MECHA_SAPIEN, 0.1, 10, YELLOW);
   DrawText("Human", 450, 340, 40, BLACK);
   DrawText("Spacelizard", 450, 390, 40, BLACK);
   DrawText("Voidcrawler", 450, 440, 40, BLACK);
   DrawText("Mecha-Sapien", 450, 490, 40, BLACK);
}
void Dprofession(){
   Color optBackground = {15, 15, 15, 175};
   Rectangle options{450, 420, 150, 200};
   DrawRectangleRounded(options, 0.1, 10, optBackground);
   Rectangle NECROMANCER {450, 420, 150, 50};
   Rectangle CRAFTSMAN {450, 470, 150, 50};
   Rectangle WIZARD {450, 520, 150, 50};
   Rectangle DUALIST {450, 570, 150, 50};

   DrawRectangleRounded(NECROMANCER, 0.1, 10, PINK);
   DrawRectangleRounded(CRAFTSMAN, 0.1, 10, RED);
   DrawRectangleRounded(WIZARD, 0.1, 10, BLUE);
   DrawRectangleRounded(DUALIST, 0.1, 10, YELLOW);
   DrawText("Necromancer", 1, 1, 1, BLACK);
   DrawText("Craftsman", 1, 1, 1, BLACK);
   DrawText("Wizard", 1, 1, 1, BLACK);
   DrawText("Dualist", 1, 1, 1, BLACK);
}
void setupChoiceEventHandler(){
   // Trying to get it so that the UI is easily toggled on/off.
   if(hover1){
      Ddifficulties();
   }
   if(hover2){
      Drace();
   }
   if(hover3){
      Dprofession();
   }
}