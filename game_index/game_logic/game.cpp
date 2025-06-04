#include <raylib.h>
#include <iostream>
#include "../include/select/Sbuttons.hpp"
#include "../include/game/loadGame.hpp"
#include "../include/game/player_stats.hpp"
#include "../include/game/ability_attributes.hpp"
#include <vector>
#include <fstream>
#include <cmath>
int loadID = 0;
Level lvl1;
Level lvl2;
constexpr float XAxisOffset = 1.0f;
const float STEP_DELAY = 0.005f;
float stepTimer = 0.0f;
void movementEventHandler(Level& lvl);
float scaleX, scaleY, scale = 1.0f;
// sprite-sheet data
int PLAYER_FRAMES = 3;
Texture2D playerTex;
const float ANIM_SPEED = 0.12f;
int currentFrame = 0;
float animTimer = 0.0f;
Camera2D cam{};
const int TILE = 16;
struct tiles{
    Texture2D load;
};
tiles purple;
tiles squiggly;
tiles Dot4;
tiles Dot4Split;
tiles background;
void loadTileTextures(){
    purple.load = LoadTexture("assets/graphics/level_graphics/tiles/tile4.png");
    squiggly.load = LoadTexture("assets/graphics/level_graphics/tiles/tile3.png");
    Dot4.load = LoadTexture("assets/graphics/level_graphics/tiles/floor2.png");
    Dot4Split.load = LoadTexture("assets/graphics/level_graphics/tiles/floor1.png");
    background.load = LoadTexture("assets/graphics/level_graphics/tiles/stdbackground.png");
}
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
    VfacingUp.pos = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
    VfacingDown.pos = LoadTexture("assets/graphics/void_crawler/void_crawler1.png");
    VfacingUpLeft.pos = LoadTexture("assets/graphics/void_crawler/void_crawler4.png");
    VfacingUpRight.pos = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
    VfacingDownLeft.pos = LoadTexture("assets/graphics/void_crawler/void_crawler2.png");
    VfacingDownRight.pos = LoadTexture("assets/graphics/void_crawler/void_crawler1.png");
}
void loadSpaceLizard(){
    SfacingUp.pos = LoadTexture("assets/graphics/");
    SfacingDown.pos = LoadTexture("assets/graphics/");
    SfacingUpLeft.pos = LoadTexture("assets/graphics/");
    SfacingUpRight.pos = LoadTexture("assets/graphics/");
    SfacingDownLeft.pos = LoadTexture("assets/graphics/");
    SfacingDownRight.pos = LoadTexture("assets/graphics/");
}
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
void readlvlData(Level& lvl){
    std::ifstream in("assets/levels/level1.txt");
    for(std::string line;std::getline(in,line);) lvl.rows.push_back(line);
    for(size_t y=0;y<lvl.rows.size();++y)
        for(size_t x=0;x<lvl.rows[y].size();++x)
            if(lvl.rows[y][x]=='p'){
                lvl.playerPos={(float)x,(float)y};
                lvl.rows[y][x]='.';
            }
}
// ── draw walls ─────────────────────────────────────────────────
void drawLevel(Level& lvl, float s){
    for(size_t y=0;y<lvl.rows.size();++y)
        for(size_t x=0;x<lvl.rows[y].size();++x){
            if(lvl.rows[y][x] == '#'){
                int px = (int)x * TILE * s;
                int py = (int)y * TILE * s;
                int sz = (int)(TILE * s);
                Rectangle mapTile = {(float)px, (float)py, (float)sz, (float)sz};
                Rectangle srcTile = {0, 0, 16, 16};
                DrawTexturePro(purple.load, srcTile, mapTile, {0, 0}, 0, WHITE);
            }
            else if(lvl.rows[y][x] == '.'){
                int px = (int)x * TILE * s;
                int py = (int)y * TILE * s;
                int sz = (int)(TILE * s);
                Rectangle mapTile = {(float)px, (float)py, (float)sz, (float)sz};
                Rectangle srcTile = {0, 0, 16, 16};
                DrawTexturePro(background.load, srcTile, mapTile, {0, 0}, 0, WHITE);
            }
        }
}
// ── spriteManager (your original ladder) ───────────────────────
int playerID;
void spriteManager(){
    if(raceSPACELIZARD){
        loadSpaceLizard();
        playerID = 1;
    }
    else if(raceVOIDCRAWLER){
        loadVoid_crawler();
        loadRollTex();
        playerID = 2;
    }
    else if(raceMECHA_SAPIEN){
        playerID = 3;
    }
    else{
        playerID = 4;
    }
}
void inputEventHandler(Level& lvl){
    bool moving = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D);
    rollAbilityLogic(lvl);
    if(rolling){
        animTimer += GetFrameTime();
        if(animTimer >= ANIM_SPEED + 0.015f){
            animTimer = 0.0f;
            currentFrame = (currentFrame + 1) % (PLAYER_FRAMES + 1);
        }  
    }
    else if(moving){
        animTimer += GetFrameTime();
        if(animTimer >= ANIM_SPEED){
            animTimer = 0.0f;
            currentFrame = (currentFrame + 1) % PLAYER_FRAMES;
        }
        movementEventHandler(lvl);
    }
    else{
        currentFrame=0;animTimer=0.0f;
        playerTex = VfacingDown.pos;
    }
}
int pPixX;
int pPixY;
int pSizeW;
int pSizeH;
Rectangle src;
Rectangle dst;
void gameLoop(Level& lvl){
    inputEventHandler(lvl);
    Vector2 playerPixCenter={
        lvl.playerPos.x*TILE*scale+(TILE*scale)/2,
        lvl.playerPos.y*TILE*scale+(TILE*scale)/2
    };
    cam.target = playerPixCenter;
    ClearBackground(BLACK);
    BeginMode2D(cam);
    // draw player sprite (18×25 frame)
    const int spriteW=18;
    const int spriteH=25;
    pSizeW = (int)(spriteW*scale);
    pSizeH = (int)(spriteH*scale);
    pPixX = (int)(lvl.playerPos.x * TILE * scale + (TILE * scale - pSizeW)/2);
    pPixY = (int)(lvl.playerPos.y * TILE * scale + (TILE * scale) - pSizeH);
    src = {currentFrame*(float)spriteW,0.0f,(float)spriteW,(float)spriteH};
    dst = {(float)pPixX, (float)pPixY, (float)pSizeW, (float)pSizeH};
    DrawRectangle(0, 0, 2000, 1000, DARKGRAY);
    drawLevel(lvl,scale);
    DrawTexturePro(playerTex, src, dst, {0,0}, 0.0f, WHITE);
    EndMode2D();
}
void loadLvl1(){
    static bool loaded=false;
    if(!loaded){
        readlvlData(lvl1);
        cam.offset = {GetScreenWidth()/2.0f,GetScreenHeight()/2.0f};
        cam.rotation = 0.0f;
        cam.zoom = scale + 2;
        spriteManager();
        loadTileTextures();
        playerTex = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
        SetTextureFilter(playerTex,TEXTURE_FILTER_POINT);
        loaded=true;
    }
    scaleX = GetScreenWidth()/(float)(lvl1.rows[0].size()*TILE);
    scaleY = GetScreenHeight()/(float)(lvl1.rows.size()*TILE);
    scale = (scaleX+scaleY)/2.0f;
    gameLoop(lvl1);
}
void loadLvl2(){
    static bool loaded=false;
    if(!loaded){
        readlvlData(lvl2);
        cam.offset = {GetScreenWidth()/2.0f,GetScreenHeight()/2.0f};
        cam.rotation = 0.0f;
        cam.zoom = scale;
        spriteManager();
        loadTileTextures();
        playerTex = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
        SetTextureFilter(playerTex,TEXTURE_FILTER_POINT);
        loaded=true;
    }
    scaleX = GetScreenWidth()/(float)(lvl2.rows[0].size()*TILE);
    scaleY = GetScreenHeight()/(float)(lvl2.rows.size()*TILE);
    scale = (scaleX+scaleY)/2.0f;
    gameLoop(lvl2);
}
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
