#include <raylib.h>
#include <iostream>
#include "../include/select/Sbuttons.hpp"
#include "../include/game/loadGame.hpp"
#include "../include/game/player_stats.hpp"
#include "../include/game/ability_attributes.hpp"
#include "../include/game/enemy_data.hpp"
#include <vector>
#include <fstream>
#include <cmath>
#include <raymath.h>
#include <cstdint>
struct Player{
    //Add in all associated values
};
struct World{
    //Add in all associated values
};
bool attacking = false;
Vector2 mouseWorld;
Vector2 playerPixCenter;
Vector2 dir;
float PLAYERWIDTH = 0.9;
float PLAYERHEIGHT = 0.4;
Vector2 spawnPos;
const float WALK_SPEED = 5.0f;
const float SQRT2 = 0.7071;
Level lvl1;
Level lvl2;
Direction currentDir;
raceP currentRace;
weapon equiped;
const float STEP_DELAY = 0.005f;
float stepTimer = 0.0f;
void movementEventHandler(Level& lvl, float);
float scaleX, scaleY, scale = 1.0f;
// sprite-sheet data
int PLAYER_FRAMES = 3;
Texture2D playerTex;
Texture2D swordTex;
Texture2D blasterTex;
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
tiles Bleft;
tiles Bright;
tiles Bup;
tiles Bdown;
void loadTileTextures(){
    purple.load = LoadTexture("assets/graphics/level_graphics/tiles/left.png");
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
    else if(cx < 0 || cx >= (int)lvl.rows[cy].size()) return true;
    return lvl.rows[cy][cx] == '#';
}
bool collisionRect(float cx, float cy, float cw, float ch, Level& lvl){
    if(isWall(cx, cy, lvl)) return true;
    else if(isWall(cx + cw, cy, lvl)) return true;
    else if(isWall(cx + cw, cy + ch, lvl)) return true;
    else if(isWall(cx, cy + ch, lvl)) return true;
    else return false;
}
bool wallBellow(float cx, float cy, Level& lvl){
    if(isWall(cx, cy + PLAYERHEIGHT * 2, lvl)) return true;
    else if(isWall(cx + PLAYERWIDTH, cy + PLAYERHEIGHT * 2, lvl)) return true;
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
struct Enemy{
    float w;
    float h;
    Texture2D spSh;
    Enemy(float w, float h, Texture2D spSh)
    : w(w), h(h), spSh(spSh) {}
};
void drawLevel(Level& lvl, float s){
    Enemy generic(10.0f, 10.0f, Dot4.load);
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
            else if(lvl.rows[y][x] == 'e'){
                DrawTexturePro(Dot4.load, srcTile, mapTile, {0, 0}, 0, WHITE);
                spawnEnemy({mapTile.x, mapTile.y}, generic.w, generic.h, 10);
            }
        }
}
// ── spriteManager (your original ladder) ───────────────────────
void spriteManager(){
    if(raceSPACELIZARD){
        loadSpaceLizard();
        currentRace = raceP::spaceLizard;
    }
    else if(raceVOIDCRAWLER){
        loadVoid_crawler();
        loadRollTex();
        currentRace = raceP::voidCrawler;
    }
    else if(raceMECHA_SAPIEN){

        currentRace = raceP::mechaSapien;
    }
    else{
        loadHuman();
        currentRace = raceP::human;
    }
}
bool rollWalkSwitch = false;
float pPixX;
float pPixY;
bool projActive;
bool rightZoom = false;
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
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        rightZoom = true;
    }
    else{
        rightZoom = false;
    }
    static float w = 5.0f;
    static float h = 5.0f;
    static float speed = 50.0f;
    static float ARCSIZE = 50.0f;
    static float range = 50.0f;
    Rectangle dest; Vector2 origin; float rotation;
    mouseWorld = GetScreenToWorld2D(GetMousePosition(), cam);
    dir = Vector2Normalize(
        Vector2Subtract(mouseWorld, {playerPixCenter.x - w/2, playerPixCenter.y - h/2}) 
    );
    const static float WEAPON_OFFSET = 12.5f;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        spawnPos = Vector2Add({playerPixCenter.x - w/2, playerPixCenter.y - h/2}, Vector2Scale(dir, WEAPON_OFFSET));
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
            spawnProjectile(spawnPos, dir, w, h, speed); 
        }
        else{
            defineDamageArea(spawnPos, range, dir, ARCSIZE);
            enemyCollisionCheck();
        }
    }
    if(projActive){
        updateRangedAttack(spawnPos, dir, w, h, speed, dt, lvl);
    }
    if(attacking){
        updateMeleeAttack(spawnPos, dir, ARCSIZE, range, lvl, dest, origin, rotation);
    }
}
void DrawSword(){
    Rectangle dest; Vector2 origin; float rotation;
    Rectangle src;
    float Xoffset; float Yoffset;
    if(currentDir == Direction::Left || currentDir == Direction::UpLeft || currentDir == Direction::DownLeft){
        rotation = 180;
        src = {0, 32, 32, -32};
        Xoffset = -9.9f * scale; Yoffset = -7.6f * scale;
    }
    else{
        src = {0, 0, 32, 32};
        rotation = 0;
        Xoffset = 9.9f * scale; Yoffset = -7.6f * scale;
    }
    float wh = 32.0f * scale;
    dest.width = wh;
    dest.height = wh;
    dest.x = playerPixCenter.x + Xoffset; dest.y = playerPixCenter.y + Yoffset;
    origin = {wh/2, wh/2};
    DrawTexturePro(swordTex, src, dest, origin, rotation, WHITE);
}
void DrawBlaster(){
    float w = 10.0f;
    float h = 10.0f;
    float WEAPON_OFFSET = 18.5f * scale;
    float rotation = atan2f(dir.y, dir.x) * RAD2DEG;
    bool flip = rotation > 90 || rotation < -90;
    float xOffset = 6.5f * scale;
    float yOffset = 5.0f * scale;
    Rectangle src;
    if (flip) src = {0, 20, 20, -20};
    else      src = {0,  0, 20,  20};
    Vector2 pivot = {
        playerPixCenter.x + xOffset + dir.x * WEAPON_OFFSET,
        playerPixCenter.y + yOffset + dir.y * WEAPON_OFFSET
    };
    Rectangle dest = {
        pivot.x - w * 0.5f,
        pivot.y - h * 0.5f,
        w,
        h
    };
    Vector2 origin = {w * 0.5f, h * 0.5f};
    DrawTexturePro(blasterTex, src, dest, origin, rotation, WHITE);
}
void DrawEquip(){
    // Weapon drawing logic for idle performance.
    switch(equiped){
        case(blaster):
            DrawBlaster();
            break;
        case(sword):
            DrawSword();
            break;
    }
}
float pSizeW;
float pSizeH;
Rectangle src;
Rectangle dst;
void gameLoop(Level& lvl){
    if(rightZoom) cam.zoom = scale * 10, equiped = blaster;
    else cam.zoom = scale * 9.5, equiped = sword;
    float dt = GetFrameTime();
    playerPixCenter = {lvl.playerPos.x*TILE*scale+(TILE*scale)/2.0f, lvl.playerPos.y*TILE*scale+(TILE*scale)/2.0f};
    cam.target = playerPixCenter;
    ClearBackground(BLACK);
    BeginMode2D(cam);
    // draw player sprite (18×25 frame)
    const float spriteW=18;
    const float spriteH=25;
    pSizeW = spriteW * scale;
    pSizeH = spriteH * scale;
    pPixX = lvl.playerPos.x * TILE * scale + (TILE * scale - pSizeW)/2;
    pPixY = lvl.playerPos.y * TILE * scale + (TILE * scale) - pSizeH;
    src = {currentFrame * (float)spriteW, 0.0f, (float)spriteW, (float)spriteH};
    dst = {pPixX, pPixY, pSizeW, pSizeH};
    drawLevel(lvl, scale);
    inputEventHandler(lvl, dt);
    DrawTexturePro(playerTex, src, dst, {0,0}, 0.0f, WHITE);
    DrawEquip();
    if(currentDir == Direction::UpLeft || currentDir == Direction::UpRight || currentDir == Direction::Up) DrawTexturePro(playerTex, src, dst, {0,0}, 0.0f, WHITE);
    if(wallBellow(lvl.playerPos.x, lvl.playerPos.y, lvl)){
        Rectangle srcTile = { 0, 0, 16, 16 };
        for(int y = 0; y < (int)lvl.rows.size(); ++y){
            for(int x = 0; x < (int)lvl.rows[y].size(); ++x){
                if(lvl.rows[y][x] == '#') {
                    Rectangle mapTile = {
                        x * TILE * scale,
                        y * TILE * scale,
                        TILE * scale,
                        TILE * scale
                    };
                    DrawTexturePro(purple.load, srcTile, mapTile, {0, 0}, 0.0f, WHITE);
                }
            }
        }
    }
    EndMode2D();
}
void preLoadTasks(Level& lvl){
    readlvlData(lvl);
    cam.offset = {GetScreenWidth()/2.0f,GetScreenHeight()/2.0f};
    cam.rotation = 0.0f;
    spriteManager();
    loadTileTextures();
    playerTex = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
    blasterTex = LoadTexture("assets/graphics/abilities/utilities/equipables/ranged/blaster.png");
    swordTex = LoadTexture("assets/graphics/abilities/utilities/equipables/melee/sword.png");
    SetTextureFilter(playerTex,TEXTURE_FILTER_POINT);
    bullets.reserve(1000);
}
void loadLvl1(){
    static bool loaded=false;
    if(!loaded){
        preLoadTasks(lvl1);
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
        preLoadTasks(lvl2);
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
    if(IsKeyDown(KEY_W)){
        y -= 1.0f;
        currentDir = Direction::Up;
    }
    if(IsKeyDown(KEY_S)){
        y += 1.0f;
        currentDir = Direction::Down;
    }
    if(IsKeyDown(KEY_A)){
        x -= 1.0f;
        if(currentDir == Direction::Up) currentDir = UpLeft;
        else if(currentDir == Direction::Down) currentDir = DownLeft;
        else currentDir = Left;
    }
    if(IsKeyDown(KEY_D)){
        x += 1.0f;
        if(currentDir == Direction::Up) currentDir = UpRight;
        else if(currentDir == Direction::Down) currentDir = DownRight;
        else currentDir = Right;
    }
    if(x != 0.0f && y != 0.0f){
        x *= SQRT2;
        y *= SQRT2;
    }
    switch(currentDir){
        case(Up):
            switch(currentRace){
                case(voidCrawler):
                    playerTex = VfacingUp.pos;
                    break;
                case(spaceLizard):
                    playerTex = SfacingUp.pos;
                    break;
                case(mechaSapien):
                    //playerTex = MfacingUp.pos;
                    break;
                default:
                    playerTex = HfacingUp.pos;
                    break;
            }
            break;
        case(Down):
            switch(currentRace){
                case(voidCrawler):
                    playerTex = VfacingDown.pos;
                    break;
                case(spaceLizard):
                    playerTex = SfacingDown.pos;
                    break;
                case(mechaSapien):
                    //playerTex = MfacingDown.pos;
                    break;
                default:
                    playerTex = HfacingDown.pos;
                    break;
            }
            break;
        case(Left):
            switch(currentRace){
                case(voidCrawler):
                    playerTex = VfacingDownLeft.pos;
                    break;
                case(spaceLizard):
                    playerTex = SfacingDownLeft.pos;
                    break;
                case(mechaSapien):
                    //playerTex = MfacingDownLeft.pos;
                    break;
                default:
                    playerTex = HfacingDownLeft.pos;
                    break;
            }
            break;
        case(Right):
            switch(currentRace){
                case(voidCrawler):
                    playerTex = VfacingDownRight.pos;
                    break;
                case(spaceLizard):
                    playerTex = SfacingDownRight.pos;
                    break;
                case(mechaSapien):
                    //playerTex = MfacingDownRight.pos;
                    break;
                default:
                    playerTex = HfacingDownRight.pos;
                    break;
            }
            break;
        case(UpLeft):
            switch(currentRace){
                case(voidCrawler):
                    playerTex = VfacingUpLeft.pos;
                    break;
                case(spaceLizard):
                    playerTex = SfacingUpLeft.pos;
                    break;
                case(mechaSapien):
                    //playerTex = MfacingUpLeft.pos;
                    break;
                default:
                    playerTex = HfacingUpLeft.pos;
                    break;
            }
            break;
        case(UpRight):
            switch(currentRace){
                case(voidCrawler):
                    playerTex = VfacingUpRight.pos;
                    break;
                case(spaceLizard):
                    playerTex = SfacingUpRight.pos;
                    break;
                case(mechaSapien):
                    //playerTex = MfacingUpRight.pos;
                    break;
                default:
                    playerTex = HfacingUpRight.pos;
                    break;
            }
            break;
        case(DownLeft):
            switch(currentRace){
                case(voidCrawler):
                    playerTex = VfacingDownLeft.pos;
                    break;
                case(spaceLizard):
                    playerTex = SfacingDownLeft.pos;
                    break;
                case(mechaSapien):
                    //playerTex = MfacingDownLeft.pos;
                    break;
                default:
                    playerTex = HfacingDownLeft.pos;
                    break;
            }
            break;
        default:
            switch(currentRace){
                case(voidCrawler):
                    playerTex = VfacingDownRight.pos;
                    break;
                case(spaceLizard):
                    playerTex = SfacingDownRight.pos;
                    break;
                case(mechaSapien):
                    //playerTex = MfacingDownRight.pos;
                    break;
                default:
                    playerTex = HfacingDownRight.pos;
                    break;
            }
            break;
    }
    float newX = lvl.playerPos.x + x * WALK_SPEED * dt;
    float newY = lvl.playerPos.y + y * WALK_SPEED * dt;
    if(!collisionRect(newX, newY, PLAYERWIDTH, PLAYERHEIGHT, lvl)){
        lvl.playerPos.x = newX;
        lvl.playerPos.y = newY;
    }
}