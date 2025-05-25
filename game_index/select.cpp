#include "raylib.h"
#include <iostream>
#include "main_select.h"
#include "Sbuttons.hpp"
#include "player_stats.hpp"
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
bool diffEASY;
bool diffMEDIUM;
bool diffHARD;
bool diffPRO_GAMER;
void diffiChoiceEventHandler(){
   if(EASYhov){
      diffMEDIUM = false;
      diffHARD = false;
      diffPRO_GAMER = false;
      diffEASY = true;
   }
   else if(MEDIUMhov){
      diffEASY = false;
      diffHARD = false;
      diffPRO_GAMER = false;
      diffMEDIUM = true;
   }
   else if(HARDhov){
      diffEASY = false;
      diffMEDIUM = false;
      diffPRO_GAMER = false;
      diffHARD = true;
   }
   else if(PRO_GAMERhov){
      diffEASY = false;
      diffMEDIUM = false; 
      diffHARD = false;
      diffPRO_GAMER = true;
   }
}
bool raceHUMAN;
bool raceSPACELIZARD;
bool raceVOIDCRAWLER;
bool raceMECHA_SAPIEN;
void raceChoiceEventHandler(){
   if(HUMANhov){
      raceSPACELIZARD = false;
      raceVOIDCRAWLER = false;
      raceMECHA_SAPIEN = false;
      raceHUMAN = true;
   }
   else if(SPACELIZARDhov){
      raceHUMAN = false;
      raceVOIDCRAWLER = false;
      raceMECHA_SAPIEN = false;
      raceSPACELIZARD = true;
   }
   else if(VOIDCRAWLERhov){
      raceSPACELIZARD = false;
      raceHUMAN = false;
      raceMECHA_SAPIEN = false;
      raceVOIDCRAWLER = true;
   }
   else if(MECHA_SAPIENhov){
      raceSPACELIZARD = false;
      raceVOIDCRAWLER = false;
      raceHUMAN = false;
      raceMECHA_SAPIEN = true;
   }
}
bool profNECROMANCER;
bool profCRAFTSMAN;
bool profWIZARD;
bool profDUALIST;
void profChoiceEventHandler(){
   if(NECROMANCERhov){
      profCRAFTSMAN = false;
      profWIZARD = false;
      profDUALIST = false;
      profNECROMANCER = true;
   }
   else if(CRAFTSMANhov){
      profCRAFTSMAN = true;
      profWIZARD = false;
      profDUALIST = false;
      profNECROMANCER = false;
   }
   else if(WIZARDhov){
      profCRAFTSMAN = false;
      profWIZARD = true;
      profDUALIST = false;
      profNECROMANCER = false;
   }
   else if(DUALISThov){
      profCRAFTSMAN = false;
      profWIZARD = false;
      profDUALIST = true;
      profNECROMANCER = false;
   }
}
Color CHOSEN = {184, 18, 57, 255};
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
   if(diffEASY){
      DrawRectangleRounded(EASY, 0.1, 10, CHOSEN);
   }
   else if(diffMEDIUM){
      DrawRectangleRounded(MEDIUM, 0.1, 10, CHOSEN);
   }
   else if(diffHARD){
      DrawRectangleRounded(HARD, 0.1, 10, CHOSEN);
   }
   else if(diffPRO_GAMER){
      DrawRectangleRounded(PRO_GAMER, 0.1, 10, CHOSEN);
   }
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
   if(raceHUMAN){
      DrawRectangleRounded(HUMAN, 0.1, 10, CHOSEN);
   }
   else if(raceSPACELIZARD){
      DrawRectangleRounded(SPACELIZARD, 0.1, 10, CHOSEN);
   }
   else if(raceVOIDCRAWLER){
      DrawRectangleRounded(VOIDCRAWLER, 0.1, 10, CHOSEN);
   }
   else if(raceMECHA_SAPIEN){
      DrawRectangleRounded(MECHA_SAPIEN, 0.1, 10, CHOSEN);
   }
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
   if(profNECROMANCER){
      DrawRectangleRounded(NECROMANCER, 0.1, 10, CHOSEN);
   }
   else if(profCRAFTSMAN){
      DrawRectangleRounded(CRAFTSMAN, 0.1, 10, CHOSEN);
   }
   else if(profWIZARD){
      DrawRectangleRounded(WIZARD, 0.1, 10, CHOSEN);
   }
   else if(profDUALIST){
      DrawRectangleRounded(DUALIST, 0.1, 10, CHOSEN);
   }
   DrawText("Necromancer", 459, 435, 20, BLACK);
   DrawText("Craftsman", 471, 485, 20, BLACK);
   DrawText("Wizard", 489, 535, 20, BLACK);
   DrawText("Dualist", 483, 585, 20, BLACK);
}
void setupChoiceEventHandler(){
   if(hover1 || optHover1){
      diffiChoiceEventHandler();
      Ddifficulties();
   }
   else if(hover2 || optHover2){
      raceChoiceEventHandler();
      Drace();
   }
   else if(hover3 || optHover3){
      profChoiceEventHandler();
      Dprofession();
   }
}
bool gameRunning = false;
void loadGameBtn(){
   Rectangle button = {600, 380, 180, 75};
   bool startHov = CheckCollisionPointRec(mousePos, button);
   bool startPressed = startHov && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
   bool startClick = startHov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
   if(startClick && (profNECROMANCER || profCRAFTSMAN || profWIZARD || profDUALIST) && (raceSPACELIZARD || raceHUMAN || raceVOIDCRAWLER || raceMECHA_SAPIEN) && (diffEASY || diffMEDIUM || diffHARD || diffPRO_GAMER)){
      //THE GAME STARTS!!!!
      gameRunning = true;
   }
   else if(startPressed){
      DrawRectangleRounded(button, 0.1, 10, GRAY);
      DrawText("LOAD GAME", 600, 380, 30, BLACK);
   }
   else if(startHov){
      DrawRectangleRounded(button, 0.1, 10, DARKGREEN);
      DrawText("LOAD GAME", 600, 380, 30, BLACK);
   }
   else{
      DrawRectangleRounded(button, 0.1, 10, GREEN);
      DrawText("LOAD GAME", 600, 380, 30, BLACK);
   }
}