#include "raylib.h"
#include <iostream>
#include "main_select.h"
#include "Sbuttons.hpp"
#include "loadGame.hpp"
#include "player_stats.hpp"
#include <vector>
#include <fstream>
void animateMove();
const float STEP_DELAY = 0.15f;
float stepTimer = 0.0f;
void movementEventHandler();

float scaleX, scaleY, scale = 1.0f;

// sprite-sheet data
const int PLAYER_FRAMES = 2;
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
        if(profCRAFTSMAN)   playerTex=LoadTexture("");                                   // add texture
        else if(profNECROMANCER) playerTex=LoadTexture("");
        else if(profWIZARD)      playerTex=LoadTexture("");
        else                     playerTex=LoadTexture("");
    }
    else if(raceVOIDCRAWLER){
        if(profCRAFTSMAN)   playerTex = LoadTexture("assets/graphics/void_crawler/void_crawler1.png");
        else if(profNECROMANCER) playerTex=LoadTexture("");
        else if(profWIZARD)      playerTex=LoadTexture("");
        else                     playerTex=LoadTexture("");
    }
    else if(raceMECHA_SAPIEN){
        if(profCRAFTSMAN)   playerTex=LoadTexture("");
        else if(profNECROMANCER) playerTex=LoadTexture("");
        else if(profWIZARD)      playerTex=LoadTexture("");
        else                     playerTex=LoadTexture("");
    }
    else{
        if(profCRAFTSMAN)   playerTex=LoadTexture("");
        else if(profNECROMANCER) playerTex=LoadTexture("");
        else if(profWIZARD)      playerTex=LoadTexture("");
        else                     playerTex=LoadTexture("");
    }
}
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
        SetTextureFilter(playerTex,TEXTURE_FILTER_POINT);
        loaded=true;
    }

    scaleX=GetScreenWidth() /(float)(lvl1.rows[0].size()*TILE);
    scaleY=GetScreenHeight()/(float)(lvl1.rows.size()*TILE);
    scale =(scaleX+scaleY)/2.0f;

    movementEventHandler();

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

    int pSizeW = (int)(spriteW*scale);
    int pSizeH = (int)(spriteH*scale);

    int pPixX = (int)(lvl1.playerPos.x*TILE*scale + (TILE*scale - pSizeW)/2);
    int pPixY = (int)(lvl1.playerPos.y*TILE*scale + (TILE*scale) - pSizeH);

    src = {currentFrame*(float)spriteW,0.0f,(float)spriteW,(float)spriteH};
    dst = {(float)pPixX,(float)pPixY,(float)pSizeW,(float)pSizeH};

    DrawTexturePro(playerTex,src,dst,{0,0},0.0f,WHITE);
    EndMode2D();
}
/*const float STEP_DELAY = 0.15f;
float stepTimer = 0.0f;*/
const float ANIMATE_DELAY = 0.01f;
float animateTimer = 0.0;
void animateMovePosXAxis(){
    animateTimer -= GetFrameTime();
    if(animateTimer > 0.0f){
        return;
    }
    dst.x += 1;
    DrawTexturePro(playerTex,src,dst,{0,0},0.0f,WHITE);
    if(dst.x >= lvl1.playerPos.x){
        animateTimer = ANIMATE_DELAY;
        return;
    }

}
// ── movement ────────────────────────────────────────────────────
void movementEventHandler(){
    stepTimer-=GetFrameTime();
    if(stepTimer>0.0f) return;
    int x=(int)lvl1.playerPos.x;
    int y=(int)lvl1.playerPos.y;
    if(IsKeyDown(KEY_W)){ 
        --y;
        playerTex = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
    }
    if(IsKeyDown(KEY_S)){
        ++y;
        playerTex = LoadTexture("assets/graphics/void_crawler/void_crawler1.png");
    }
    if(IsKeyDown(KEY_A)){
        --x; 
        playerTex = LoadTexture("assets/graphics/void_crawler/void_crawler2.png");
    }
    if(IsKeyDown(KEY_D)){
        ++x;
        playerTex = LoadTexture("assets/graphics/void_crawler/void_crawler4.png");
    }
    if(!isWall(x,y)) lvl1.playerPos={(float)x,(float)y};
    stepTimer=STEP_DELAY;
}
