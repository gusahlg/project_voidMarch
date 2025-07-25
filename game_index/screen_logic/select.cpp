#include <raylib.h>
#include <iostream>
#include "../include/game/player_stats.hpp"
#include <cstdint>
#include <vector>
#include <cmath>
bool raceSPACELIZARD = false;
bool raceVOIDCRAWLER = false;
bool raceMECHA_SAPIEN = false;
bool raceHUMAN = true;

bool profNECROMANCER = true;
bool profCRAFTSMAN = false;
bool profWIZARD = false;
bool profDUALIST = false;
enum class prof : std::uint8_t{Necromancer, Craftsman, Wizard, Dualist};
prof currentProf;
enum class race : std::uint8_t{Spacelizard, Voidcrawler, Mecha_sapien, Human};
struct button{
    Rectangle rect;
    Texture tex;
    enum state : std::uint8_t{Inactive, Hover, Pressed, Clicked};
    state currentState = state::Inactive;
    void determineState(Vector2 mousePos){
        bool inside = mousePos.x - rect.x < rect.width && mousePos.y - rect.y < rect.height;
        if(!inside) return;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            currentState = state::Pressed;
        }
        else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
            currentState = state::Clicked;
        }
        else currentState = state::Hover;
    }
    void drawBtn(){
        //Change to use textures later
        switch(currentState){
            case state::Hover: DrawRectangleRec(rect, BLACK); break;
            case state::Pressed: DrawRectangleRec(rect, PINK); break;
            case state::Inactive: DrawRectangleRec(rect, GREEN); break;
            default: /*output*/; break;
        }
    }
    button(Rectangle rectangle, Texture texture)
    : rect(rectangle), tex(texture) {}
};
std::vector<button> buttons;
void updateButtons(Vector2 mousePos){
    for(auto& b : buttons){
        b.determineState(mousePos);
        switch(b.currentState){
            case b.state::Hover: /*Do something*/ break;
            case b.state::Pressed: /*Do something*/ break;
            case b.state::Clicked: /*Do something*/ break;
        }
        b.drawBtn();
        b.currentState = b.state::Inactive;
    }
}
void BtnAssembly(std::vector<Rectangle> rects, Texture texture){
    for(const Rectangle r : rects){
        buttons.emplace_back(r, texture);
    }
}