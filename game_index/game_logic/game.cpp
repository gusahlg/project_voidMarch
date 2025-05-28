#include <raylib.h>
#include <iostream>
#include "../include/select/Sbuttons.hpp"
#include "../include/game/loadGame.hpp"
#include "../include/game/player_stats.hpp"
#include "../include/game/ability_attributes.hpp"
#include <vector>
#include <fstream>
#include <cmath>
Level lvl1;
Level lvl2;
constexpr float XAxisOffset = 1.0f;
const float STEP_DELAY = 0.005f;
float stepTimer = 0.0f;
void movementEventHandler(Level& lvl);
float scaleX, scaleY, scale = 1.0f;
// sprite-sheet data
const int PLAYER_FRAMES = 3;
Texture2D playerTex;
const float ANIM_SPEED = 0.12f;
int currentFrame = 0;
float animTimer = 0.0f;
Camera2D cam{};
const int TILE = 16;
bool isWall(int cx, int cy, Level& lvl){
    if(cy < 0 || cy >= (int)lvl.rows.size()) return true;
    if(cx < 0 || cx >= (int)lvl.rows[cy].size()) return true;
    return lvl.rows[cy][cx] == '#';
}
bool wallAbove(Level& lvl){
    if(isWall(lvl.playerPos.x, lvl.playerPos.y - 1, lvl)){
        return true;  
    }
    else{
        return false;
    }
}
// ── level loading ───────────────────────────────────────────────
std::ifstream in("../assets/levels/level1.txt");
void readlvlData(Level& lvl){
    for(std::string line;std::getline(in,line);) lvl.rows.push_back(line);
    for(size_t y=0;y<lvl.rows.size();++y)
        for(size_t x=0;x<lvl.rows[y].size();++x)
            if(lvl.rows[y][x]=='p'){
                lvl.playerPos={(float)x,(float)y};
                lvl.rows[y][x]='.';
            }
}
// ── draw walls ─────────────────────────────────────────────────
void drawLevel(const Level& lvl,float s){
    for(size_t y=0;y<lvl.rows.size();++y)
        for(size_t x=0;x<lvl.rows[y].size();++x){
            if(lvl.rows[y][x]!='#') continue;
            int px = (int)x * TILE * s;
            int py = (int)y * TILE * s;
            int sz = (int)(TILE * s);
            DrawRectangle(px, py, sz, sz, DARKGRAY);
        }
}
// ── spriteManager (your original ladder) ───────────────────────
struct void_crawler{
    Texture2D pos;
};
void_crawler VfacingUp;
void_crawler VfacingDown;
void_crawler VfacingUpLeft;
void_crawler VfacingUpRight;
void_crawler VfacingDownLeft;
void_crawler VfacingDownRight;
struct Space_lizard{
    Texture2D pos;
};
Space_lizard SfacingUp;
Space_lizard SfacingDown;
Space_lizard SfacingUpLeft;
Space_lizard SfacingUpRight;
Space_lizard SfacingDownLeft;
Space_lizard SfacingDownRight;
void loadVoid_crawler(){
    VfacingUp.pos = LoadTexture("../assets/graphics/void_crawler/void_crawler3.png");
    VfacingDown.pos = LoadTexture("../assets/graphics/void_crawler/void_crawler1.png");
    VfacingUpLeft.pos = LoadTexture("../assets/graphics/void_crawler/void_crawler4.png");
    VfacingUpRight.pos = LoadTexture("../assets/graphics/void_crawler/void_crawler3.png");
    VfacingDownLeft.pos = LoadTexture("../assets/graphics/void_crawler/void_crawler2.png");
    VfacingDownRight.pos = LoadTexture("../assets/graphics/void_crawler/void_crawler1.png");
}
void loadSpaceLizard(){
    SfacingUp.pos = LoadTexture("../assets/graphics/");
    SfacingDown.pos = LoadTexture("../assets/graphics/");
    SfacingUpLeft.pos = LoadTexture("../assets/graphics/");
    SfacingUpRight.pos = LoadTexture("../assets/graphics/");
    SfacingDownLeft.pos = LoadTexture("../assets/graphics/");
    SfacingDownRight.pos = LoadTexture("../assets/graphics/");
}
int playerID;
void spriteManager(){
    if(raceSPACELIZARD){
        loadSpaceLizard();
        playerID = 1;
    }
    else if(raceVOIDCRAWLER){
        loadVoid_crawler();
        playerID = 2;
    }
    else if(raceMECHA_SAPIEN){
        playerID = 3;
    }
    else{
        playerID = 4;
    }
}
int pPixX;
int pPixY;
int pSizeW;
int pSizeH;
// ── level-1 per-frame ───────────────────────────────────────────
Rectangle src;
Rectangle dst;
void loadLvl1(){
    static bool loaded=false;
    if(!loaded){
        readlvlData(lvl1);
        cam.offset = {GetScreenWidth()/2.0f,GetScreenHeight()/2.0f};
        cam.rotation = 0.0f;
        cam.zoom = 3.0f;
        spriteManager();
        SetTextureFilter(playerTex,TEXTURE_FILTER_POINT);
        loaded=true;
    }
    scaleX=GetScreenWidth() /(float)(lvl1.rows[0].size()*TILE);
    scaleY=GetScreenHeight()/(float)(lvl1.rows.size()*TILE);
    scale =(scaleX+scaleY)/2.0f;
    bool moving = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D);
    if(moving){
        animTimer += GetFrameTime();
        if(animTimer >= ANIM_SPEED){
            animTimer -= ANIM_SPEED;
            currentFrame = (currentFrame+1)%PLAYER_FRAMES;
        }
        movementEventHandler(lvl1);
    }
    else{
        currentFrame=0;animTimer=0.0f;
    }
    Vector2 playerPixCenter={
        lvl1.playerPos.x*TILE*scale+(TILE*scale)/2,
        lvl1.playerPos.y*TILE*scale+(TILE*scale)/2
    };
    cam.target = playerPixCenter;
    abilityInputHandler(lvl1);
    ClearBackground(BLACK);
    BeginMode2D(cam);
    // draw player sprite (18×25 frame)
    const int spriteW=18;
    const int spriteH=25;
    pSizeW = (int)(spriteW*scale);
    pSizeH = (int)(spriteH*scale);
    pPixX = (int)(lvl1.playerPos.x * TILE * scale + (TILE * scale - pSizeW)/2);
    pPixY = (int)(lvl1.playerPos.y * TILE * scale + (TILE * scale) - pSizeH);
    src = {currentFrame*(float)spriteW,0.0f,(float)spriteW,(float)spriteH};
    dst = { (float)pPixX, (float)pPixY, (float)pSizeW, (float)pSizeH };
    DrawTexturePro(playerTex, src, dst, {0,0}, 0.0f, WHITE);
    drawLevel(lvl1,scale);
    if(wallAbove(lvl1)){
        DrawTexturePro(playerTex, src, dst, {0,0}, 0.0f, WHITE);
    }
    EndMode2D();
}
bool lookingUp;
bool lookingUpLeft;
bool lookingUpRight;
bool lookingLeft;
bool lookingRight;
bool lookingDown;
bool lookingDownLeft;
bool lookingDownRight;
int loadID;
void movementEventHandler(Level& lvl){
    stepTimer-=GetFrameTime();
    if(stepTimer > 0.0f) return;
    float x=(float)lvl.playerPos.x;
    float y=(float)lvl.playerPos.y;;
    if(IsKeyDown(KEY_W)){
        if(IsKeyDown(KEY_A)){
            y -= 0.035355f;
            x -= 0.035355f;
            if(loadID != 1){
                if(playerID == 1){
                    playerTex = SfacingUpLeft.pos;
                }
                else if(playerID == 2){
                    playerTex = VfacingUpLeft.pos;
                }
                else if(playerID == 3){

                }
                else{

                }
                loadID = 1;
            }
        }
        else if(IsKeyDown(KEY_D)){
            y -= 0.035355f;
            x += 0.035355f;
            if(loadID != 1){
                if(playerID == 1){
                    playerTex = SfacingUpRight.pos;
                }
                else if(playerID == 2){
                    playerTex = VfacingUpRight.pos;
                }
                else if(playerID == 3){

                }
                else{
                    
                }
                loadID = 1;
            }
        }
        else{
            y -= 0.05f;
            if(loadID != 1){
                if(playerID == 1){
                    playerTex = SfacingUp.pos;
                }
                else if(playerID == 2){
                    playerTex = VfacingUp.pos;
                }
                else if(playerID == 3){

                }
                else{
                    
                }                
                loadID = 1;
            }
        }
    }
    else if(IsKeyDown(KEY_S)){
        if(IsKeyDown(KEY_A)){
            y += 0.035355f;
            x -= 0.035355f;
            if(loadID != 2){
                if(playerID == 1){
                    playerTex = SfacingDownLeft.pos;
                }
                else if(playerID == 2){
                    playerTex = VfacingDownLeft.pos;
                }
                else if(playerID == 3){

                }
                else{
                    
                }
                loadID = 2;
            }
        }
        else if(IsKeyDown(KEY_D)){
            y += 0.035355f;
            x += 0.035355f;
            if(loadID != 2){
                if(playerID == 1){
                    playerTex = SfacingDownRight.pos;
                }
                else if(playerID == 2){
                    playerTex = VfacingDownRight.pos;
                }
                else if(playerID == 3){

                }
                else{
                    
                }
                loadID = 2;
            }
        }
        else{
            y += 0.05f;
            if(loadID != 2){
                if(playerID == 1){
                    playerTex = SfacingDown.pos;
                }
                else if(playerID == 2){
                    playerTex = VfacingDown.pos;
                }
                else if(playerID == 3){

                }
                else{
                    
                }
                loadID = 2;
            }
        }
    }
    else if(IsKeyDown(KEY_A)){
        x -= 0.05f;
        if(loadID != 3){
            if(playerID == 1){
                playerTex = SfacingDownLeft.pos;
            }
            else if(playerID == 2){
                playerTex = VfacingDownLeft.pos;
            }
            else if(playerID == 3){

            }
            else{
                    
            }
            loadID = 3;
        }  
    }
    else if(IsKeyDown(KEY_D)){
        x += 0.05f;
        if(loadID != 4){
            if(playerID == 1){
                playerTex = SfacingDownRight.pos;
            }
            else if(playerID == 2){
                playerTex = VfacingDownRight.pos;
            }
            else if(playerID == 3){

            }
            else{
                    
            }
            loadID = 4; 
        }
    }
    int cx = (int)std::floorf(x);
    int cy = (int)std::floorf(y);
    if(!isWall(cx, cy, lvl) && !isWall(cx + XAxisOffset, cy, lvl)){
        lvl.playerPos={(float)x,(float)y};
    }
    stepTimer = STEP_DELAY;
}
