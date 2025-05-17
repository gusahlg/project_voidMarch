#pragma once
#include "raylib.h"
class Sbuttons {
public:
    int BtnID; 
    Rectangle button;
    Sbuttons(int id, Rectangle rect)
        : BtnID(id), button(rect) {}
    void update() {
        switch (BtnID) {
        case 1:
            DrawRectangleRounded(button, 0.2f, 100, DARKGRAY);
            break;       
        case 2:
            // TODO: choose a colour or graphic for button 2
            break;
        case 3:
            // TODO: choose a colour or graphic for button 3
            break;
        default:
            break;
        }
    }
};
