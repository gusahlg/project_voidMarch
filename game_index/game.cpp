#include "raylib.h"
#include <iostream>
#include "main_select.h"
#include "Sbuttons.hpp"
#include "loadGame.hpp"
#include "player_stats.hpp"
#include <vector>
#include <fstream>
#include <cmath>
constexpr float BBOX_HALF_W = 1.0f;
const float STEP_DELAY = 0.005f;
float stepTimer = 0.0f;
void movementEventHandler();
float scaleX, scaleY, scale = 1.0f;
// sprite-sheet data
const int PLAYER_FRAMES = 3;
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
bool isWall(int cx, int cy){
    if(cy < 0 || cy >= (int)lvl1.rows.size()) return true;
    if(cx < 0 || cx >= (int)lvl1.rows[cy].size()) return true;
    return lvl1.rows[cy][cx] == '#';
}
// ── level loading ───────────────────────────────────────────────
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
// ── draw walls ─────────────────────────────────────────────────
void drawLevel(const Level& lvl,float s){
    for(size_t y=0;y<lvl.rows.size();++y)
        for(size_t x=0;x<lvl.rows[y].size();++x){
            if(lvl.rows[y][x]!='#') continue;
            int px=(int)x*TILE*s;
            int py=(int)y*TILE*s;
            int sz=(int)(TILE*s);
            DrawRectangle(px,py,sz,sz,DARKGRAY);
        }
}
// ── spriteManager (your original ladder) ───────────────────────
void spriteManager(){
    if(raceSPACELIZARD){

    }
    else if(raceVOIDCRAWLER){
        playerTex = LoadTexture("assets/graphics/void_crawler/void_crawler1.png");
    }
    else if(raceMECHA_SAPIEN){

    }
    else{

    }
}
int pPixX;
int pPixY;
int pSizeW;
int pSizeH;
struct void_crawler{
    Texture2D pos;
};
void_crawler facingUp;
void_crawler facingDown;
void_crawler facingUpLeft;
void_crawler facingUpRight;
void_crawler facingDownLeft;
void_crawler facingDownRight;
// ── level-1 per-frame ───────────────────────────────────────────
Rectangle src;
Rectangle dst;
void loadLvl1(){
    static bool loaded=false;
    if(!loaded){
        readlvlData();
        cam.offset={GetScreenWidth()/2.0f,GetScreenHeight()/2.0f};
        cam.rotation=0.0f;
        cam.zoom=1.5f;
        spriteManager();
        facingUp.pos = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
        facingDown.pos = LoadTexture("assets/graphics/void_crawler/void_crawler1.png");
        facingUpLeft.pos = LoadTexture("assets/graphics/void_crawler/void_crawler4.png");
        facingUpRight.pos = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
        facingDownLeft.pos = LoadTexture("assets/graphics/void_crawler/void_crawler2.png");
        facingDownRight.pos = LoadTexture("assets/graphics/void_crawler/void_crawler1.png");
        SetTextureFilter(playerTex,TEXTURE_FILTER_POINT);
        loaded=true;
    }
    scaleX=GetScreenWidth() /(float)(lvl1.rows[0].size()*TILE);
    scaleY=GetScreenHeight()/(float)(lvl1.rows.size()*TILE);
    scale =(scaleX+scaleY)/2.0f;
    bool moving=IsKeyDown(KEY_W)||IsKeyDown(KEY_A)||IsKeyDown(KEY_S)||IsKeyDown(KEY_D);
    if(moving){
        animTimer+=GetFrameTime();
        if(animTimer>=ANIM_SPEED){
            animTimer-=ANIM_SPEED;
            currentFrame=(currentFrame+1)%PLAYER_FRAMES;
        }
    }else{currentFrame=0;animTimer=0.0f;}
    Vector2 playerPixCenter={
        lvl1.playerPos.x*TILE*scale+(TILE*scale)/2,
        lvl1.playerPos.y*TILE*scale+(TILE*scale)/2
    };
    cam.target=playerPixCenter;
    ClearBackground(BLACK);
    BeginMode2D(cam);
    drawLevel(lvl1,scale);
    // draw player sprite (18×25 frame)
    const int spriteW=18;
    const int spriteH=25;
    pSizeW = (int)(spriteW*scale);
    pSizeH = (int)(spriteH*scale);
    pPixX = (int)(lvl1.playerPos.x*TILE*scale + (TILE*scale - pSizeW)/2);
    pPixY = (int)(lvl1.playerPos.y*TILE*scale + (TILE*scale) - pSizeH);
    src = {currentFrame*(float)spriteW,0.0f,(float)spriteW,(float)spriteH};
    movementEventHandler();
    dst = { (float)pPixX, (float)pPixY, (float)pSizeW, (float)pSizeH };
    DrawTexturePro(playerTex, src, dst, {0,0}, 0.0f, WHITE);
    EndMode2D();
}
void movementEventHandler(){
    stepTimer-=GetFrameTime();
    if(stepTimer > 0.0f) return;
    float x=(float)lvl1.playerPos.x;
    float y=(float)lvl1.playerPos.y;
    static int loadID;
    static bool up;
    if(IsKeyDown(KEY_W)){
        if(IsKeyDown(KEY_A)){
            y -= 0.05f;
            x -= 0.05f;
            if(loadID != 1){
                playerTex = facingUpLeft.pos;
                loadID = 1;
            }
        }
        else if(IsKeyDown(KEY_D)){
            y -= 0.05f;
            x += 0.05f;
            if(loadID != 1){
                playerTex = facingUpRight.pos;
                loadID = 1;
            }
        }
        else{
            y -= 0.05f;
            if(loadID != 1){
                playerTex = facingUp.pos;
                loadID = 1;
            }
        }
        up = true;
    }
    else if(IsKeyDown(KEY_S)){
        if(IsKeyDown(KEY_A)){
            y += 0.05f;
            x -= 0.05f;
            if(loadID != 2){
                playerTex = facingDownLeft.pos;
                loadID = 2;
            }
        }
        else if(IsKeyDown(KEY_D)){
            y += 0.05f;
            x += 0.05f;
            if(loadID != 2){
                playerTex = facingDownRight.pos;
                loadID = 2;
            }
        }
        else{
            y += 0.05f;
            if(loadID != 2){
                playerTex = facingDown.pos;
                loadID = 2;
            }
        }
        up = false;
    }
    else if(IsKeyDown(KEY_A)){
        x -= 0.05f;
        if(loadID != 3){
            if(up){
                playerTex = facingUpLeft.pos;
                loadID = 3;
            }
            else{   
                playerTex = facingDownLeft.pos;
                loadID = 3;
            }
        }  
    }
    else if(IsKeyDown(KEY_D)){
        x += 0.05f;
        if(loadID != 4){
            if(up){
                playerTex = facingUpRight.pos;
                loadID = 4;
            }
            else{
                playerTex = facingDownRight.pos;
                loadID = 4;
            }
        }
    }
    int cx = (int)std::floorf(x);
    int cy = (int)std::floorf(y);
    if(!isWall(cx, cy) && !isWall(cx, cx + BBOX_HALF_W)){
        lvl1.playerPos={(float)x,(float)y};
    }
    stepTimer=STEP_DELAY;
}
/*bool XYCollCheck(int tx){
    int left = tx;
    int right = tx + BBOX_HALF_W;
    return isWall(left, right);
}*/
