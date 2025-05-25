#include "raylib.h"
#include <iostream>
#include "main_select.h"
#include "Sbuttons.hpp"
#include "loadGame.hpp"
#include <vector>
#include <fstream>
const float STEP_DELAY = 0.15f;
float stepTimer = 0.0f;
void movementEventHandler();
float scaleX;
float scaleY;
float scale = (scaleX + scaleY)/2;
//Spritesheet animations
const int PLAYER_FRAMES = 4;
Texture2D playerTex;
const float ANIM_SPEED = 0.12f;
int currentFrame = 0;
float animTimer = 0.0f;

Camera2D cam{};
const int TILE = 16;
struct Level{
    std::vector<std::string> rows;
    Vector2 playerPos;
};
Level lvl1;
Level lvl2;
bool isWall(int tx, int ty){
    if(ty < 0 || ty >= (int)lvl1.rows.size())     return true;
    if(tx < 0 || tx >= (int)lvl1.rows[ty].size()) return true;
    return lvl1.rows[ty][tx] == '#';
}
std::ifstream in("assets/levels/level1.txt");
bool lvlLoaded = false;
void readlvlData(){
for(std::string line; std::getline(in, line);){
    lvl1.rows.push_back(line);
}
for(size_t y = 0; y < lvl1.rows.size(); ++y){
    for(size_t x = 0; x < lvl1.rows[y].size(); ++x){
        if(lvl1.rows[y][x] == 'p'){
            lvl1.playerPos = {(float)x, (float)y};
            lvl1.rows[y][x] = '.';
        }
    }
}
}
void drawLevel(const Level& lvl){
    scaleX = GetScreenWidth() / (float)(lvl.rows[0].size() * TILE);
    scaleY = GetScreenHeight() / (float)(lvl.rows.size() * TILE);
    for(size_t y = 0; y < lvl.rows.size(); ++y){
        for(size_t x = 0; x < lvl.rows[y].size(); ++x){
            char cell = lvl.rows[y][x];
            //This converts tile coords to pixel coords.
            int px = static_cast<int>(x) * TILE * scale;
            int py = static_cast<int>(y) * TILE * scale;
            int tileHeight = static_cast<int>(TILE * scale);
            int tileWidth = static_cast<int>(TILE * scale);
            if(cell == '#'){
                DrawRectangle(px, py, tileWidth, tileHeight, DARKGRAY);
            }
        }
    }
}
void loadLvl1(){
    scale = (scaleY + scaleX)/2;
    static bool loaded = false;
    //System for sprite changing clock:
    bool playerMoving = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D);
    if(playerMoving){
        animTimer += GetFrameTime();
        if(animTimer >= ANIM_SPEED){
            animTimer -= ANIM_SPEED;
            currentFrame = (currentFrame + 1) % PLAYER_FRAMES;
        }
    }
    else{
        currentFrame = 0;
        animTimer = 0.0f;
    }
    //Below is for loading the level.
    if(!loaded){
        readlvlData();
        cam.offset = {scale, scale}; // Screen center
        cam.rotation = 0.0f;
        cam.zoom = 1.0f;
        playerTex = LoadTexture("assets/graphics/test_sprite.png");
        SetTextureFilter(playerTex, TEXTURE_FILTER_POINT);
        loaded = true;
    }
    movementEventHandler();
    scaleX = GetScreenWidth() / (float)(lvl1.rows[0].size() * TILE);
    scaleY = GetScreenHeight() / (float)(lvl1.rows.size() * TILE);
    scale = (scaleX + scaleY)/2;
    Vector2 playerCoords = {
        lvl1.playerPos.x * TILE * scale + (TILE * scale)/2,
        lvl1.playerPos.y * TILE * scale + (TILE * scale)/2
    };
    cam.target = playerCoords;
    ClearBackground(BLACK);
    BeginMode2D(cam);
    drawLevel(lvl1);
    //Below is for sprites drawing.
    int pPixX = (int)lvl1.playerPos.x * TILE * scale;
    int pPixY = (int)lvl1.playerPos.y * TILE * scale;
    int pSize  = (int)(TILE * scale);
    Rectangle src = { currentFrame * 16.0f, 0, 16, 16 };
    Rectangle dst = { (float)pPixX, (float)pPixY, (float)pSize, (float)pSize };
    DrawTexturePro(playerTex, src, dst, {0,0}, 0.0f, WHITE);
    EndMode2D();
}
void movementEventHandler(){
    stepTimer -= GetFrameTime();
    if(stepTimer > 0.0f) return;
    int x = (int)lvl1.playerPos.x;
    int y = (int)lvl1.playerPos.y;
    if(IsKeyDown(KEY_W)){
        --y;
    }
    if(IsKeyDown(KEY_S)){
        ++y;           
    }
    if(IsKeyDown(KEY_A)){
        --x;
    }
    if(IsKeyDown(KEY_D)){
        ++x;
    }
    if(!isWall(x, y)){
        lvl1.playerPos = {(float)x, (float)y};
    }
    stepTimer = STEP_DELAY;
}