#pragma once
#include <raylib.h>
#include <iostream>
extern Color Chover;
extern Color Cpressed;
extern Color CB;
extern Color HNON;
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
extern bool gameRunning;
extern Rectangle Ubutton1;
extern Rectangle Ubutton2;
extern Rectangle Ubutton3;
void checkForBtnState();
void setupChoiceEventHandler();
void Drace();
void Ddifficulties();
void Dprofession();
void loadGameBtn();
class Sbuttons{
public:
    int BtnID; 
    Rectangle button;
    Sbuttons(int id, Rectangle rect)
        : BtnID(id), button(rect) {}
    void update(){
        int btnX = 300;
        int btnY = 275;
        if(hover1 || hover2 || hover3 || optHover1 || optHover2 || optHover3){
            if(BtnID == 1 && hover1 || optHover1){
                if(pressed1){
                    DrawRectangleRounded(button, 0.2, 100, Cpressed);
                    DrawRectangleRounded(Ubutton2, 0.2, 100, HNON);
                    DrawRectangleRounded(Ubutton3, 0.2, 100, HNON);
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);
                    DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);
                    DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK);
                }
                else{
                    DrawRectangleRounded(button, 0.2, 100, Chover);
                    DrawRectangleRounded(Ubutton2, 0.2, 100, HNON);
                    DrawRectangleRounded(Ubutton3, 0.2, 100, HNON);
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);  
                    DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);   
                    DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK); 
                    setupChoiceEventHandler();
                }
            }
            else if(BtnID == 2 && hover2 || optHover2){
                if(pressed2){
                    DrawRectangleRounded(button, 0.2, 100, Cpressed);
                    DrawRectangleRounded(Ubutton1, 0.2, 100, HNON);
                    DrawRectangleRounded(Ubutton3, 0.2, 100, HNON);
                    DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);
                    DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK);
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);  
                }
                else{
                    DrawRectangleRounded(button, 0.2, 100, Chover);  
                    DrawRectangleRounded(Ubutton1, 0.2, 100, HNON);
                    DrawRectangleRounded(Ubutton3, 0.2, 100, HNON);
                    DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);     
                    DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK);    
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);  
                    setupChoiceEventHandler();         
                }
            }
            else if(BtnID == 3 && hover3 || optHover3){
                if(pressed3){
                    DrawRectangleRounded(button, 0.2, 100, Cpressed);
                    DrawRectangleRounded(Ubutton1, 0.2, 100, HNON);
                    DrawRectangleRounded(Ubutton2, 0.2, 100, HNON);
                    DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK);
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);  
                    DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);
                }
                else{
                    DrawRectangleRounded(button, 0.2, 100, Chover);  
                    DrawRectangleRounded(Ubutton1, 0.2, 100, HNON);
                    DrawRectangleRounded(Ubutton2, 0.2, 100, HNON);
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
                break;       
            case 2:
                DrawRectangleRounded(button, 0.2f, 100, CB);
                DrawText("RACE", btnX + 45, btnY + 80, 23, BLACK);
                break;
            case 3:
                DrawRectangleRounded(button, 0.2f, 100, CB);
                DrawText("PROFESSION", btnX, btnY + 160, 23, BLACK);
                break;
            }
        }
        loadGameBtn();
    }
};
extern Sbuttons Btn1;
extern Sbuttons Btn2;
extern Sbuttons Btn3;