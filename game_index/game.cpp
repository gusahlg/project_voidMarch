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
// New level objects are put here:
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
// Takes the file data and converts it into easy to understand x and y coordinates for the player and then stores int in the lvl1 variable for player position, right?
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
            int px = static_cast<int>(x) * TILE * scaleX;
            int py = static_cast<int>(y) * TILE * scaleY;
            int tileHeight = static_cast<int>(TILE * scaleY);
            int tileWidth = static_cast<int>(TILE * scaleX);
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
    Vector2 playerCoords = {
        lvl1.playerPos.x * TILE * scaleX + (TILE * scaleX)/2,
        lvl1.playerPos.y * TILE * scaleY + (TILE * scaleY)/2
    };
    cam.target = playerCoords;
    ClearBackground(BLACK);
    BeginMode2D(cam);
    drawLevel(lvl1);
    int pPixX = (int)lvl1.playerPos.x * TILE * scaleX;
    int pPixY = (int)lvl1.playerPos.y * TILE * scaleY;
    int pSizeW = (int)(TILE * scaleX);
    int pSizeH = (int)(TILE * scaleY);
    DrawRectangle(pPixX, pPixY, pSizeW, pSizeH, RED);
    EndMode2D();
}
void movementEventHandler(){
    int x = (int)lvl1.playerPos.x;
    int y = (int)lvl1.playerPos.y;
    if(IsKeyPressed(KEY_W)){
        --y;
    }
    if(IsKeyPressed(KEY_S)){
        ++y;
    }
    if(IsKeyPressed(KEY_A)){
        --x;
    }
    if(IsKeyPressed(KEY_D)){
        ++x;
    }
    //Using the info from drawLevel, (using a boolean that I've not declared yet) only have the update happen if the data returns that moving wouldn't make a collision.
    if(!isWall(x, y)){
        lvl1.playerPos = {(float)x, (float)y};
    }
}