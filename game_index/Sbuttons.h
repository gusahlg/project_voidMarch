#pragma once
#include "raylib.h"
class Sbuttons {
public:
    int BtnID; 
    Rectangle button;
    Sbuttons(int id, Rectangle rect)
        : BtnID(id), button(rect) {}
    void update(){
        switch(BtnID){
        case 1:
            DrawRectangleRounded(button, 0.2f, 100, PINK);
            break;       
        case 2:
            DrawRectangleRounded(button, 0.2f, 100, PINK);
            break;
        case 3:
            DrawRectangleRounded(button, 0.2f, 100, PINK);
            break;
        default:
            break;
        }
    }
};
