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
extern Vector2 mousePos;
void checkForBtnState();
class Sbuttons{
public:
    int BtnID; 
    Rectangle button;
    Sbuttons(int id, Rectangle rect)
        : BtnID(id), button(rect) {}
    void update(){
        int btnX = 565;
        int btnY = 275;
        if(hover1 || hover2 || hover3){
            if(hover1 && BtnID == 1){
                if(pressed1){
                    DrawRectangleRounded(button, 0.2, 100, Cpressed);
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);
                }
                else if(clicked1){

                }
                else{
                    DrawRectangleRounded(button, 0.2, 100, Chover);
                    DrawText("DIFFICULTY", btnX, btnY, 23, BLACK);                
                }
            }
            if(hover2 && BtnID == 2){
                if(pressed2){
                    DrawRectangleRounded(button, 0.2, 100, Cpressed);
                    DrawText("DIFFICULTY", btnX, btnY + 80, 23, BLACK);
                }
                else if(clicked2){

                }
                else{
                    DrawRectangleRounded(button, 0.2, 100, Chover);  
                    DrawText("DIFFICULTY", btnX, btnY + 80, 23, BLACK);                  
                }
            }
            if(hover3 && BtnID == 3){
                if(pressed3){
                    DrawRectangleRounded(button, 0.2, 100, Cpressed);
                    DrawText("DIFFICULTY", btnX, btnY + 160, 23, BLACK);
                }
                else if(clicked3){

                }
                else{
                    DrawRectangleRounded(button, 0.2, 100, Chover);  
                    DrawText("DIFFICULTY", btnX, btnY + 160, 23, BLACK);                  
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
                DrawText("DIFFICULTY", btnX, btnY + 80, 23, BLACK);
                break;
            case 3:
                DrawRectangleRounded(button, 0.2f, 100, CB);
                DrawText("DIFFICULTY", btnX, btnY + 160, 23, BLACK);
                break;
            }
        }
    }
};
extern Sbuttons Btn1;
extern Sbuttons Btn2;
extern Sbuttons Btn3;
