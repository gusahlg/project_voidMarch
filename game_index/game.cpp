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
float scaleX, scaleY, scale = 1.0f;
// sprite sheet data 
const int PLAYER_FRAMES = 4;
Texture2D playerTex;
const float ANIM_SPEED = 0.12f;
int   currentFrame = 0;
float animTimer    = 0.0f;

Camera2D cam{};
const int TILE = 16;
struct Level{
    std::vector<std::string> rows;
    Vector2 playerPos;
};
Level lvl1;
Level lvl2;
bool isWall(int tx,int ty){
    if(ty<0||ty>=(int)lvl1.rows.size()) return true;
    if(tx<0||tx>=(int)lvl1.rows[ty].size()) return true;
    return lvl1.rows[ty][tx]=='#';
}
// level loading
std::ifstream in("assets/levels/level1.txt");
void readlvlData(){
    for(std::string line;std::getline(in,line);) lvl1.rows.push_back(line);
    for(size_t y=0;y<lvl1.rows.size();++y)
        for(size_t x=0;x<lvl1.rows[y].size();++x)
            if(lvl1.rows[y][x]=='p'){
                lvl1.playerPos={(float)x,(float)y};
                lvl1.rows[y][x]='.';
            }
}
// draw one level (walls only)
void drawLevel(const Level& lvl,float s){   // ADDED: pass uniform scale
    for(size_t y=0;y<lvl.rows.size();++y)
        for(size_t x=0;x<lvl.rows[y].size();++x){
            if(lvl.rows[y][x]!='#') continue;
            int px=(int)x*TILE*s;
            int py=(int)y*TILE*s;
            int sz=(int)(TILE*s);
            DrawRectangle(px,py,sz,sz,DARKGRAY);
        }
}
// main per-frame routine for level 1 
void loadLvl1(){
    static bool loaded=false;
    if(!loaded){
        readlvlData();
        cam.offset={GetScreenWidth()/2.0f,GetScreenHeight()/2.0f};
        cam.rotation=0.0f;
        cam.zoom=1.0f;
        playerTex=LoadTexture("assets/graphics/test_sprite.png");
        SetTextureFilter(playerTex,TEXTURE_FILTER_POINT);
        loaded=true;
    }
    // compute scales once per frame
    scaleX = GetScreenWidth() /(float)(lvl1.rows[0].size()*TILE);
    scaleY = GetScreenHeight()/(float)(lvl1.rows.size()*TILE);
    scale  = (scaleX+scaleY)/2.0f;                                
    movementEventHandler();
    // animation timer
    bool moving = IsKeyDown(KEY_W)||IsKeyDown(KEY_A)||IsKeyDown(KEY_S)||IsKeyDown(KEY_D);
    if(moving){
        animTimer+=GetFrameTime();
        if(animTimer>=ANIM_SPEED){
            animTimer-=ANIM_SPEED;
            currentFrame=(currentFrame+1)%PLAYER_FRAMES;
        }
    }else{currentFrame=0;animTimer=0.0f;}
    // update camera to player centre
    Vector2 playerPixCenter={
        lvl1.playerPos.x*TILE*scale+(TILE*scale)/2,
        lvl1.playerPos.y*TILE*scale+(TILE*scale)/2
    };
    cam.target=playerPixCenter;
    ClearBackground(BLACK);
    BeginMode2D(cam);
    drawLevel(lvl1, scale);
    // draw player sprite
    int pPixX=(int)lvl1.playerPos.x*TILE*scale;
    int pPixY=(int)lvl1.playerPos.y*TILE*scale;
    int pSize=(int)(TILE*scale);
    Rectangle src={currentFrame*16.0f,0,16,16};      
    Rectangle dst={(float)pPixX,(float)pPixY,(float)pSize,(float)pSize};
    DrawTexturePro(playerTex,src,dst,{0,0},0.0f,WHITE);   
    EndMode2D();
}
// handle tile-step movement with cooldown
void movementEventHandler(){
    stepTimer-=GetFrameTime();
    if(stepTimer>0.0f) return;
    int x=(int)lvl1.playerPos.x;
    int y=(int)lvl1.playerPos.y;
    if(IsKeyDown(KEY_W)) --y;
    else if(IsKeyDown(KEY_S)) ++y;
    else if(IsKeyDown(KEY_A)) --x;
    else if(IsKeyDown(KEY_D)) ++x;
    if(!isWall(x,y)){
        lvl1.playerPos={(float)x,(float)y};
    }
    stepTimer=STEP_DELAY;
}