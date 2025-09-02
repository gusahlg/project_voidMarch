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
    int stages{};
    Rectangle drawBox;
    Color tint;
    float clock;
    Star(Texture2D tex, Rectangle src, Rectangle drawBox, Color tint, float clock)
    : tex(tex), src(src), drawBox(drawBox), tint(tint), clock(clock) {}
};/*794*/
std::vector<Star> stars;
// PLEASE FIX STARS ONCE IT'S ALL IN SAME SPRITESHEET!!!
void generateStars(int screenWidth, int screenHeight){
    static Texture2D Star1 = LoadTexture("assets/UI/screen_interface/backgrounds/StarSheet1.png");
    static Texture2D Star2 = LoadTexture("assets/UI/screen_interface/backgrounds/StarSheet2.png");
    static Texture2D Star3 = LoadTexture("assets/UI/screen_interface/backgrounds/StarSheet3.png");
    static Texture2D fullStarSheet = LoadTexture("assets/UI/screen_interface/backgrounds/StarSheet3.png");
    // Generate new stars until maximum star amount is reached:
    for(int i = 0; i < (int)(500 - stars.size()); ++i){
        Texture2D tex = fullStarSheet;
        int x = rng_int(0, screenWidth);
        int y = rng_int(0, screenHeight);
        int width  = rng_int(0.01, 20);
        int height = rng_int(0.01, 20);
        Color tint = WHITE;
        if(rng_int(1,3) != 1) tint = {(unsigned char)rng_int(0,255), (unsigned char)rng_int(0,255), (unsigned char)rng_int(0,255), (unsigned char)rng_int(0,255)};
        float clock = rng_float(0.6, 2.5);
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
            case 0: s.src.x = 5.f;  s.src.y = float(5 * rng_int(0, (s.tex.height/5) - 1)); break;
            case 1: s.src.x = 10.f; s.src.y = float(5 * rng_int(0, (s.tex.height/5) - 1)); break;
        }
        ++s.stages;
        s.clock = rng_float(3, 8);
}
    stars.erase(
        std::remove_if(
            stars.begin(), stars.end(), [](auto& s){
                return s.stages >= 2;
            }
        ),
        stars.end());
}
void drawStars(){
    for(const Star& s : stars){
        DrawTexturePro(s.tex, s.src, s.drawBox, {0,0}, 0, s.tint);
    }
}
// TODO: Fix UI system so that it works well with ONE spritesheet!
void mainPreLoadTasks(float screenWidth, float screenHeight){
    static Texture2D button = LoadTexture("assets/ui/screen_interface/buttons/blueButton.png");
    const float btnX = screenWidth/2.f-button.width/3/2.f;
    const float btnY = screenHeight/2.f-button.height/3/2.f;
    mainManager.emplaceButton(Rectangle{btnX,btnY,float(button.width),float(button.height)},button,[]{currentScreen=screen::Select;});
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