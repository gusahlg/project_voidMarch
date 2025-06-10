#include <raylib.h>
#include <iostream>
#include "../include/select/Sbuttons.hpp"
#include "../include/game/loadGame.hpp"
#include "../include/game/player_stats.hpp"
#include "../include/game/ability_attributes.hpp"
#include <vector>
#include <fstream>
#include <cmath>
#include <raymath.h>
float PLAYERWIDTH = 0.9;
float PLAYERHEIGHT = 0.4;
bool V = false;
bool S = false;
bool H = false;
const float WALK_SPEED = 5.0f;
const float SQRT2 = 0.7071;
int loadID = 0;
Level lvl1;
Level lvl2;
constexpr float XAxisOffset = 1.0f;
const float STEP_DELAY = 0.005f;
float stepTimer = 0.0f;
void movementEventHandler(Level& lvl, float);
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
struct Human{
    Texture2D pos;
};
Human HfacingUp;
Human HfacingDown;
Human HfacingUpLeft;
Human HfacingUpRight;
Human HfacingDownLeft;
Human HfacingDownRight;
void loadVoid_crawler(){
    VfacingUp.pos = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
    VfacingDown.pos = LoadTexture("assets/graphics/void_crawler/void_crawler1.png");
    VfacingUpLeft.pos = LoadTexture("assets/graphics/void_crawler/void_crawler4.png");
    VfacingUpRight.pos = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
    VfacingDownLeft.pos = LoadTexture("assets/graphics/void_crawler/void_crawler2.png");
    VfacingDownRight.pos = LoadTexture("assets/graphics/void_crawler/void_crawler1.png");
}
void loadSpaceLizard(){
    SfacingUp.pos = LoadTexture("assets/graphics/space_lizard/");
    SfacingDown.pos = LoadTexture("assets/graphics/space_lizard/");
    SfacingUpLeft.pos = LoadTexture("assets/graphics/space_lizard/");
    SfacingUpRight.pos = LoadTexture("assets/graphics/space_lizard/");
    SfacingDownLeft.pos = LoadTexture("assets/graphics/space_lizard/");
    SfacingDownRight.pos = LoadTexture("assets/graphics/space_lizard/");
}
void loadHuman(){
    HfacingUp.pos = LoadTexture("assets/graphics/human/human2.png");
    HfacingDown.pos = LoadTexture("assets/graphics/human/human3.png");
    HfacingUpLeft.pos = LoadTexture("assets/graphics/human/human1.png");
    HfacingUpRight.pos = LoadTexture("assets/graphics/human/human2.png");
    HfacingDownLeft.pos = LoadTexture("assets/graphics/human/human4.png");
    HfacingDownRight.pos = LoadTexture("assets/graphics/human/human3.png");
}
bool isWall(float cx, float cy, Level& lvl){
    if(cy < 0 || cy >= (int)lvl.rows.size()) return true;
    if(cx < 0 || cx >= (int)lvl.rows[cy].size()) return true;
    return lvl.rows[cy][cx] == '#';
}
bool collisionRect(float cx, float cy, float cw, float ch, Level& lvl){
    if(isWall(cx, cy, lvl)) return true;
    if(isWall(cx + cw, cy, lvl)) return true;
    if(isWall(cx + cw, cy + ch, lvl)) return true;
    if(isWall(cx, cy + ch, lvl)) return true;
    else return false;
}
bool wallBellow(float cx, float cy, Level& lvl){
    if(isWall(cx, cy + PLAYERHEIGHT * 2, lvl)) return true;
    if(isWall(cx + PLAYERWIDTH, cy + PLAYERHEIGHT * 2, lvl)) return true;
    else return false;
}
// ── level loading ───────────────────────────────────────────────
void readlvlData(Level& lvl){
    std::ifstream in("assets/levels/level1.txt");
    for(std::string line;std::getline(in,line);) lvl.rows.push_back(line);
    for(size_t y=0;y<lvl.rows.size();++y)
        for(size_t x=0;x<lvl.rows[y].size();++x)
            if(lvl.rows[y][x] == 'p'){
                lvl.playerPos = {(float)x, (float)y};
                lvl.rows[y][x]='.';
            }
}
// ── draw walls ─────────────────────────────────────────────────
void drawLevel(Level& lvl, float s){
    for(size_t y=0;y<lvl.rows.size();++y)
        for(size_t x=0;x<lvl.rows[y].size();++x){
            float px = x * TILE * s;
            float py = y * TILE * s;
            float sz = (TILE * s);
            Rectangle mapTile = {(float)px, (float)py, (float)sz, (float)sz};
            Rectangle srcTile = {0, 0, 16, 16};
            if(lvl.rows[y][x] == '#'){
                DrawTexturePro(purple.load, srcTile, mapTile, {0, 0}, 0, WHITE);
            }
            else if(lvl.rows[y][x] == '.'){
                DrawTexturePro(background.load, srcTile, mapTile, {0, 0}, 0, WHITE);
            }
            else if(lvl.rows[y][x] == 'x'){
                DrawTexturePro(squiggly.load, srcTile, mapTile, {0, 0}, 0, WHITE);
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
        loadHuman();
        playerID = 4;
    }
    switch(playerID){
        case(1):
            S = true;
            break;
        case(2):
            V = true;
            break;
        case(3):
            break;
        case(4):
            H = true;
    }
}
bool rollWalkSwitch = false;
int pPixX;
int pPixY;
void inputEventHandler(Level& lvl, float dt){
    bool moving = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D);
    float const delay = 0.9f;
    float static rollTimer = 0.0f;
    rollTimer += GetFrameTime();
    // Related to movement
    if(!rolling){
        Ox = lvl.playerPos.x;
        Oy = lvl.playerPos.y;
    }
    if(IsKeyPressed(KEY_SPACE) && rollTimer >= delay || rolling){
        updateRoll(lvl, dt);
        animTimer += GetFrameTime();
        if(animTimer >= ANIM_SPEED + 0.015f){
            animTimer = 0.0f;
            currentFrame = (currentFrame + 1) % (PLAYER_FRAMES + 1);
        }
        if(rollTimer >= delay){
            rollTimer = 0.0f;
        }  
        rollWalkSwitch = true;
    }
    else if(moving){
        animTimer += GetFrameTime();
        if(animTimer >= ANIM_SPEED){
            animTimer = 0.0f;
            currentFrame = (currentFrame + 1) % PLAYER_FRAMES;
        }
        movementEventHandler(lvl, dt);
        rollWalkSwitch = false;
    }
    else{
        currentFrame=0;animTimer=0.0f;
        playerTex = VfacingDown.pos;
    }
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || 1 == 1){
        Vector2 pixPos = {pPixX, pPixY};
        Vector2 dir = Vector2Normalize({
            GetMouseX() - pPixX,
            GetMouseY() - pPixY
        });
        updateRangedAttack(pixPos, dir, dt, lvl);
    }
}
int pSizeW;
int pSizeH;
Rectangle src;
Rectangle dst;
void gameLoop(Level& lvl){
    float dt = GetFrameTime();
    Vector2 playerPixCenter = {lvl.playerPos.x*TILE*scale+(TILE*scale)/2, lvl.playerPos.y*TILE*scale+(TILE*scale)/2};
    cam.target = playerPixCenter;
    ClearBackground(BLACK);
    BeginMode2D(cam);
    // draw player sprite (18×25 frame)
    const int spriteW=18;
    const int spriteH=25;
    pSizeW = (int)(spriteW * scale);
    pSizeH = (int)(spriteH * scale);
    pPixX = (int)(lvl.playerPos.x * TILE * scale + (TILE * scale - pSizeW)/2);
    pPixY = (int)(lvl.playerPos.y * TILE * scale + (TILE * scale) - pSizeH);
    src = {currentFrame * (float)spriteW, 0.0f, (float)spriteW, (float)spriteH};
    dst = {(float)pPixX, (float)pPixY, (float)pSizeW, (float)pSizeH};
    drawLevel(lvl, scale);
    inputEventHandler(lvl, dt);
    DrawTexturePro(playerTex, src, dst, {0,0}, 0.0f, WHITE);
    if(wallBellow(lvl.playerPos.x, lvl.playerPos.y, lvl)){
    Rectangle srcTile = { 0, 0, 16, 16 };
    for(int y = 0; y < (int)lvl.rows.size(); ++y) {
        for(int x = 0; x < (int)lvl.rows[y].size(); ++x){
            if(lvl.rows[y][x] == '#') {
                Rectangle mapTile = {
                    x * TILE * scale,
                    y * TILE * scale,
                    TILE * scale,
                    TILE * scale
                };
                DrawTexturePro(purple.load, srcTile, mapTile, { 0, 0 }, 0.0f, WHITE);
            }
        }
    }
}
    EndMode2D();
}
void loadLvl1(){
    static bool loaded=false;
    if(!loaded){
        readlvlData(lvl1);
        cam.offset = {GetScreenWidth()/2.0f,GetScreenHeight()/2.0f};
        cam.rotation = 0.0f;
        cam.zoom = scale * 3.5;
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
        cam.zoom = scale * 3.5;
        spriteManager();
        loadTileTextures();
        playerTex = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
        SetTextureFilter(playerTex, TEXTURE_FILTER_POINT);
        loaded=true;
    }
    scaleX = GetScreenWidth()/(float)(lvl2.rows[0].size()*TILE);
    scaleY = GetScreenHeight()/(float)(lvl2.rows.size()*TILE);
    scale = (scaleX + scaleY)/2.0f;
    gameLoop(lvl2);
}
void movementEventHandler(Level& lvl, float dt){
    float x = 0.0f;
    float y = 0.0f;
    bool up = false; bool down = false; bool left = false; bool right = false;
    if(IsKeyDown(KEY_W)){
        y -= 1.0f;
        up = true;
        loadID = 1;
    }
    if(IsKeyDown(KEY_S)){
        y += 1.0f;
        right = true;
        loadID = 2;
    }
    if(IsKeyDown(KEY_A)){
        x -= 1.0f;
        left = true;
        loadID = 3;
    }
    if(IsKeyDown(KEY_D)){
        x += 1.0f;
        right = true;
        loadID = 4;
    }
    if(x != 0.0f && y != 0.0f){
        x *= SQRT2;
        y *= SQRT2;
    }
    if(up){
        if(left){
            if(V){
                playerTex = VfacingUpLeft.pos;
            }
            else if(H){
                playerTex = HfacingUpLeft.pos;
            }
        }
        else if(right){
            if(V){
                playerTex = VfacingUpRight.pos;
            }
            else if(H){
                playerTex = HfacingUpRight.pos;
            }
        }
        else{
            if(V){
                playerTex = VfacingUp.pos;
            }
            else if(H){
                playerTex = HfacingUp.pos;
            }
        }
    }
    else if(down){
        if(left){
            if(V){
                playerTex = VfacingDownLeft.pos;
            }
            else if(H){
                playerTex = HfacingDownLeft.pos;
            }
        }
        else if(right){
            if(V){
                playerTex = VfacingDownRight.pos;
            }
            else if(H){
                playerTex = HfacingDownRight.pos;
            }
        }
        else{
            if(V){
                playerTex = VfacingDown.pos;
            }
            else if(H){
                playerTex = HfacingDown.pos;
            }
        }
    }
    else if(left){
        if(V){
            playerTex = VfacingDownLeft.pos;
        }
        else if(H){
            playerTex = HfacingDownLeft.pos;
        }
    }
    else if(right){
        if(V){
            playerTex = VfacingDownRight.pos;
        }
        else if(H){
            playerTex = HfacingDownRight.pos;
        }
    }
    float newX = lvl.playerPos.x + x * WALK_SPEED * dt;
    float newY = lvl.playerPos.y + y * WALK_SPEED * dt;
    if(!collisionRect(newX, newY, PLAYERWIDTH, PLAYERHEIGHT, lvl)){
        lvl.playerPos.x = newX;
        lvl.playerPos.y = newY;
    }
}