#pragma once
#include "raylib.h"
#include <iostream>
extern Color Chover;
extern Color Cpressed;
extern Color CB;
extern bool hover1;
extern bool hover2;
extern bool hover3;
extern bool pressed1;
extern bool clicked1;
extern bool pressed2;
extern bool clicked2;
extern bool pressed3;
extern bool clicked3;
extern bool Sbtn1Click;
extern bool Sbtn2Click;
extern bool Sbtn3Click;
extern Vector2 mousePos;
extern bool optHover1;
extern bool optHover2;
extern bool optHover3;
extern bool EASYhov;
extern bool MEDIUMhov;
extern bool HARDhov;
extern bool PRO_GAMERhov;
extern bool HUMANhov;
extern bool SPACELIZARDhov;
extern bool VOIDCRAWLERhov;
extern bool MECHA_SAPIENhov;
extern bool NECROMANCERhov;
extern bool CRAFTSMANhov;
extern bool WIZARDhov;
extern bool DUALISThov;
void checkForBtnState();
void setupChoiceEventHandler();
void Drace();
void Ddifficulties();
void Dprofession();
class Sbuttons{
public:
    int BtnID; 
    Rectangle button;
    Sbuttons(int id, Rectangle rect)
        : BtnID(id), button(rect) {}
    void update(){
        int btnX = 300;
        int btnY = 275;
        if(BtnID == 1 && hover1 || hover2 || hover3 || optHover1 || optHover2 || optHover3){
            if(hover1 || optHover1 || optHover2 || optHover3){
                if(pressed1){
                    DrawRectangleRounded(button, 0.2, 100, Cpressed);
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);
                    DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);
                    DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK);
                }
                else{
                    DrawRectangleRounded(button, 0.2, 100, Chover);
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);  
                    DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);   
                    DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK); 
                    setupChoiceEventHandler();          
                }
            }
            else if(hover2 || optHover1 || optHover2 || optHover3){
                if(pressed2){
                    DrawRectangleRounded(button, 0.2, 100, Cpressed);
                    DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);
                    DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK);
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);  
                }
                else{
                    DrawRectangleRounded(button, 0.2, 100, Chover);  
                    DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);     
                    DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK);    
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);  
                    setupChoiceEventHandler();         
                }
            }
            else if(hover3 || optHover3){
                if(pressed3){
                    DrawRectangleRounded(button, 0.2, 100, Cpressed);
                    DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK);
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);  
                    DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);
                }
                else{
                    DrawRectangleRounded(button, 0.2, 100, Chover);  
                    DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK);    
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK); 
                    DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);  
                    setupChoiceEventHandler();             
                }
            }
        }
        else{
            switch(BtnID){
            case 1:
                DrawRectangleRounded(button, 0.2f, 100, CB);
                DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);
                setupChoiceEventHandler();
                break;       
            case 2:
                DrawRectangleRounded(button, 0.2f, 100, CB);
                DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);
                setupChoiceEventHandler();
                break;
            case 3:
                DrawRectangleRounded(button, 0.2f, 100, CB);
                DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK);
                setupChoiceEventHandler();
                break;
            }
        }
    }
};
extern Sbuttons Btn1;
extern Sbuttons Btn2;
extern Sbuttons Btn3;
