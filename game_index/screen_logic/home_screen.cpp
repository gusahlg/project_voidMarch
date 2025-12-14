#include <raylib.h>
#include <iostream>
#include "../include/game/player_stats.hpp"
#include "../include/screen_load/screen_constants.hpp"
#include <cstdint>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <random>
// Can communicate with main.cpp:
#include "../include/screen_load/select_main.hpp"
// Gives easy access to all standard ui features:
#include "../include/standardUI/manager.hpp"
// The files' local ui manager:
ui::Manager mainManager;
static thread_local std::mt19937 rng{
    std::random_device{}()
};
int rng_int(int low, int high){
    std::uniform_int_distribution<int> dist(low, high);
    return dist(rng);
}
float rng_float(float low, float high){
    std::uniform_real_distribution<float> dist(low, high);
    return dist(rng);
}
// Make a better color picker code, make smaller stars more likely, make stars more commonly circular, make star variations.
struct Star{
    Texture2D tex;
    Rectangle src;
    int stages=0;
    Rectangle drawBox;
    Color tint;
    int clock;
    Star(Texture2D tex, Rectangle src, Rectangle drawBox, Color tint, float clock)
    : tex(tex), src(src), drawBox(drawBox), tint(tint), clock(clock) {}
};
std::vector<Star> stars;
void generateStars(int screenWidth, int screenHeight){
    static Texture2D fullStarSheet = LoadTexture("assets/UI/screen_interface/backgrounds/fullStarSheet.png");
    // Generate new stars until maximum star amount is reached:
    for(int i = 0; i < (int)(100000 - stars.size()); ++i){
        Texture2D tex = fullStarSheet;
        int x = rng_int(0, screenWidth);
        int y = rng_int(0, screenHeight);
        int width  = rng_int(0.1, 1);
        int height = rng_int(0.1, 1);
        if(rng_int(0,1) == 1){
            width = rng_int(1, 2);
            height = rng_int(1, 2);
        }
        else if(rng_int(0,1) == 1){
            width = rng_int(2, 4);
            height = rng_int(2, 4);
        }
        Color tint;
        if(rng_int(1,3) == 1){
            tint = {(unsigned char)rng_int(0,255), (unsigned char)rng_int(0,255), (unsigned char)rng_int(0,255), (unsigned char)rng_int(0,255)};
        } 
        else{tint = WHITE;}
        int clock = rng_int(0, 5);
        static const int rows = fullStarSheet.height / 5;
        int row  = rng_int(0, std::max(0, rows - 1));
        Rectangle src = {0.f, float(row * 5), 5.f, 5.f};
        Rectangle drawBox = {(float)x, (float)y, (float)width, (float)height};
        stars.emplace_back(tex, src, drawBox, tint, clock);
    }
}
void updateStars(){
    for(Star& s : stars){
        s.clock -= GetFrameTime();
        if(s.clock > 0) continue;
        switch(s.stages){
            case 0: s.src.x = 5.f; break;
            case 1: s.src.x = 10.f; break;
            case 2: s.src.x = 15.f; break;
            case 3: s.src.x = 20.f; break;
        }
        ++s.stages;
        s.clock = rng_int(500, 10000);
}
    stars.erase(
        std::remove_if(
            stars.begin(), stars.end(), [](auto& s){
                return s.stages >= 3;
            }
        ),
        stars.end());
}
void drawStars(){
    for(const Star& s : stars){
        DrawTexturePro(s.tex, s.src, s.drawBox, {0,0}, 0, s.tint);
    }
}
void mainPreLoadTasks(float screenWidth, float screenHeight){
    static Texture2D button = LoadTexture("assets/UI/screen_interface/buttons/blueButton.png");
    const float btnX = screenWidth/2.f;
    const float btnY = screenHeight/2.f;
    mainManager.emplaceButton(Vector2{btnX,btnY},button,[]{currentScreen=screen::Select;});
}
void loadMainScreen(Vector2 mousePos, float screenWidth, float screenHeight){
    static bool loaded = false;
    if(!loaded){
        mainPreLoadTasks(screenWidth, screenHeight);
        loaded = true;
    }
    mainManager.updateAll(mousePos);
    mainManager.drawAll();
    generateStars((int)screenWidth, (int)screenHeight);
    updateStars();
    drawStars();
}
