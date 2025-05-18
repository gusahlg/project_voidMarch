#include "raylib.h"
#include <iostream>
#include "main_select.h"
#include "Sbuttons.hpp"
void selectScr::selectScrUpdate(){
   Rectangle selectPanel{128, 80, 1024, 600};
   DrawRectangleRounded(selectPanel, 0.2, 100, DARKGRAY);
}
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
// Adding so that stuff happens on click
bool Sbtn1Click = false;
bool Sbtn2Click = false;
bool Sbtn3Click = false;
// Making the options for the game setup, will be triggered if something is clicked.
void Ddifficulties(){
   // Local variables for handling interface size (a rectangle that's around the options).
   Color optBackground = {15, 15, 15, 175};
   Rectangle options {128, 80, 1024, 600};
   DrawRectangleRounded(options, 0.1, 10, optBackground);
   // The different options, add in so that the choices can be clicked on.
   Rectangle EASY {};
   Rectangle MEDIUM {};
   Rectangle HARD {};
   Rectangle PRO_GAMER {};
   DrawText("EASY", 1, 1, 1, BLACK);
   DrawText("MEDIUM", 1, 1, 1, BLACK);
   DrawText("HARD", 1, 1, 1, BLACK);
   DrawText("PRO GAMER", 1, 1, 1, BLACK);
}
void Drace(){
   Color optBackground = {15, 15, 15, 175};
   Rectangle options{128, 80, 1024, 600};
   DrawRectangleRounded(options, 0.1, 10, optBackground);
   Rectangle HUMAN {};
   Rectangle SPACELIZARD {};
   Rectangle VOIDCRAWLER {};
   Rectangle MECHA_SAPIEN {};
   DrawText("Human", 1, 1, 1, BLACK);
   DrawText("Spacelizard", 1, 1, 1, BLACK);
   DrawText("Voidcrawler", 1, 1, 1, BLACK);
   DrawText("Mecha-Sapien", 1, 1, 1, BLACK);
}
void Dprofession(){
   Color optBackground = {15, 15, 15, 175};
   Rectangle options{128, 80, 1024, 600};
   DrawRectangleRounded(options, 0.1, 10, optBackground);
   Rectangle NECROMANCER {};
   Rectangle CRAFTSMAN {};
   Rectangle WIZARD {};
   Rectangle DUALIST {};
   DrawText("Necromancer", 1, 1, 1, BLACK);
   DrawText("Craftsman", 1, 1, 1, BLACK);
   DrawText("Wizard", 1, 1, 1, BLACK);
   DrawText("Dualist", 1, 1, 1, BLACK);
}
void setupChoiceEventHandler(){
   // Trying to get it so that the UI is easily toggled on/off.
   if(Sbtn1Click){
      Ddifficulties();
   }
   if(Sbtn2Click){
      Drace();
   }
   if(Sbtn3Click){
      Dprofession();
   }
}