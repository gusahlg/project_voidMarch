#include "raylib.h"
#include <iostream>
#include "main_select.h"
#include "Sbuttons.hpp"
#include "loadGame.hpp"
#include <vector>
#include <fstream>
void movementEventHandler();
float scaleX;
float scaleY;
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
    float scale = (scaleY + scaleX)/2;
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
    static bool loaded = false;
    if(!loaded){
        readlvlData();
        cam.offset = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f}; // Screen center
        cam.rotation = 0.0f;
        cam.zoom = 1.0f;
        loaded = true;
    }
    movementEventHandler();
    scaleX = GetScreenWidth() / (float)(lvl1.rows[0].size() * TILE);
    scaleY = GetScreenHeight() / (float)(lvl1.rows.size() * TILE);
    float scale = (scaleX + scaleY)/2;
    Vector2 playerCoords = {
        lvl1.playerPos.x * TILE * scaleX + (TILE * scale)/2,
        lvl1.playerPos.y * TILE * scaleY + (TILE * scale)/2
    };
    cam.target = playerCoords;
    ClearBackground(BLACK);
    BeginMode2D(cam);
    drawLevel(lvl1);
    int pPixX = (int)lvl1.playerPos.x * TILE * scale;
    int pPixY = (int)lvl1.playerPos.y * TILE * scale;
    int pSizeW = (int)(TILE * scale);
    int pSizeH = (int)(TILE * scale);
    DrawRectangle(pPixX, pPixY, pSizeW, pSizeH, RED);
    EndMode2D();
}
void movementEventHandler(){
    int x = (int)lvl1.playerPos.x;
    int y = (int)lvl1.playerPos.y;
    bool checking = false;
    while(!checking){
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
    }
    if(!isWall(x, y)){
        lvl1.playerPos = {(float)x, (float)y};
    }
}