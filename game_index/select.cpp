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
Rectangle Ubutton1 {300, 260, 150, 50};
Rectangle Ubutton2 {300, 340, 150, 50};
Rectangle Ubutton3 {300, 420, 150, 50};
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
Color HNON = {111, 44, 115, 255};
bool EASYhov;
bool MEDIUMhov;
bool HARDhov;
bool PRO_GAMERhov;
void Ddifficulties(){
   Color optBackground = {15, 15, 15, 175};
   Rectangle options {450, 260, 150, 200};
   DrawRectangleRounded(options, 0.1, 10, optBackground);
   optHover1 = CheckCollisionPointRec(mousePos, options);
   Rectangle EASY {450, 260, 150, 50};
   EASYhov = CheckCollisionPointRec(mousePos, EASY) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   Rectangle MEDIUM {450, 310, 150, 50};
   MEDIUMhov = CheckCollisionPointRec(mousePos, MEDIUM) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   Rectangle HARD {450, 360, 150, 50};
   HARDhov = CheckCollisionPointRec(mousePos, HARD) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   Rectangle PRO_GAMER {450, 410, 150, 50};
   PRO_GAMERhov = CheckCollisionPointRec(mousePos, PRO_GAMER) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   DrawText("EASY", 496, 273, 24, BLACK);
   DrawText("MEDIUM", 482, 323, 24, BLACK);
   DrawText("HARD", 496, 373, 24, BLACK);
   DrawText("PRO GAMER", 460, 423, 24, BLACK);
}
bool HUMANhov;
bool SPACELIZARDhov;
bool VOIDCRAWLERhov;
bool MECHA_SAPIENhov;
void Drace(){
   Color optBackground = {15, 15, 15, 175};
   Rectangle options{450, 340, 150, 200};
   DrawRectangleRounded(options, 0.1, 10, optBackground);
   optHover2 = CheckCollisionPointRec(mousePos, options);
   Rectangle HUMAN {450, 340, 150, 50};
   HUMANhov = CheckCollisionPointRec(mousePos, HUMAN) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   Rectangle SPACELIZARD {450, 390, 150, 50};
   SPACELIZARDhov = CheckCollisionPointRec(mousePos, SPACELIZARD) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   Rectangle VOIDCRAWLER {450, 440, 150, 50};
   VOIDCRAWLERhov = CheckCollisionPointRec(mousePos, VOIDCRAWLER) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   Rectangle MECHA_SAPIEN {450, 490, 150, 50};
   MECHA_SAPIENhov = CheckCollisionPointRec(mousePos, MECHA_SAPIEN) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   DrawText("Human", 489, 353, 24, BLACK);
   DrawText("Spacelizard", 466, 406, 18, BLACK);
   DrawText("Voidcrawler", 466, 456, 18, BLACK);
   DrawText("Mecha-Sapien", 460, 506, 18, BLACK);
}
bool NECROMANCERhov;
bool CRAFTSMANhov;
bool WIZARDhov;
bool DUALISThov;
void Dprofession(){
   Color optBackground = {15, 15, 15, 175};
   Rectangle options{450, 420, 150, 200};
   DrawRectangleRounded(options, 0.1, 10, optBackground);
   optHover3 = CheckCollisionPointRec(mousePos, options);
   Rectangle NECROMANCER {450, 420, 150, 50};
   NECROMANCERhov = CheckCollisionPointRec(mousePos, NECROMANCER) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   Rectangle CRAFTSMAN {450, 470, 150, 50};
   CRAFTSMANhov = CheckCollisionPointRec(mousePos, CRAFTSMAN) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   Rectangle WIZARD {450, 520, 150, 50};
   WIZARDhov = CheckCollisionPointRec(mousePos, WIZARD) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   Rectangle DUALIST {450, 570, 150, 50};
   DUALISThov = CheckCollisionPointRec(mousePos, DUALIST) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   DrawText("Necromancer", 459, 435, 20, BLACK);
   DrawText("Craftsman", 471, 485, 20, BLACK);
   DrawText("Wizard", 489, 535, 20, BLACK);
   DrawText("Dualist", 483, 585, 20, BLACK);
}
void setupChoiceEventHandler(){
   if(hover1 || optHover1){
      Ddifficulties();
   }
   else if(hover2 || optHover2){
      Drace();
   }
   else if(hover3 || optHover3){
      Dprofession();
   }
}
//Make it so that the buttons actually do something you morron.
void diffiChoiceEventHandler(){
   if(EASYhov){

   }
   else if(MEDIUMhov){

   }
   else if(HARDhov){

   }
   else if(PRO_GAMERhov){

   }
}
void raceChoiceEventHandler(){

}
void profChoiceEventHandler(){

}
void characterCreationChoiceEventHandler(){

}