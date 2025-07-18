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
#include "../include/data/stats/player_data.hpp"
#include "../include/data/stats/player.hpp"
// Cool idea: Add in so that the constructor takes in the current level as input to determine which variations to use.
struct TileSet{
    Texture2D wall;
    Texture2D floor;
    //Add more if needed.
    TileSet(Texture2D wall, Texture2D floor)
    : wall(wall), floor(floor) {}
};
TileSet tiles{/*wall =*/ {}, /*floor =*/ {} };
bool attacking = false;
Vector2 mouseWorld;
Vector2 playerPixCenter;
Vector2 dir;
float PLAYERWIDTH = 0.9;
float PLAYERHEIGHT = 0.4;
Vector2 spawnPos;
constexpr float WALK_SPEED = 5.0f;
constexpr float SQRT2 = 0.70710678119;
Level lvl1;
Level lvl2;
Direction currentDir;
raceP currentRace;
weapon equipped;
constexpr float STEP_DELAY = 0.005f;
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
void loadTileTextures(){
    const std::vector<std::pair<const char*, Texture2D*>> todo = {
        {"assets/graphics/level_graphics/tiles/left.png",         &tiles.wall},
        {"assets/graphics/level_graphics/tiles/stdbackground.png",&tiles.floor}
        // Add more textures here and in struct.
    };
    for (auto [file, out] : todo) *out = LoadTexture(file);
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
    SfacingDownLeft.pos = LoadTexture("assets/graphics/space_lizard/SpaceLizardLeft.png");
    SfacingDownRight.pos = LoadTexture("assets/graphics/space_lizard/SpaceLizardRight.png");
}
void loadHuman(){
    HfacingUp.pos = LoadTexture("assets/graphics/human/human2.png");
    HfacingDown.pos = LoadTexture("assets/graphics/human/human3.png");
    HfacingUpLeft.pos = LoadTexture("assets/graphics/human/human1.png");
    HfacingUpRight.pos = LoadTexture("assets/graphics/human/human2.png");
    HfacingDownLeft.pos = LoadTexture("assets/graphics/human/human4.png");
    HfacingDownRight.pos = LoadTexture("assets/graphics/human/human3.png");
}
bool R2CollCheck(Rectangle FirstRec, Rectangle SecondRec){
    if(FirstRec.x < SecondRec.x + SecondRec.width &&
       FirstRec.x + FirstRec.width > SecondRec.x &&
       FirstRec.y < SecondRec.y + SecondRec.height &&
       FirstRec.y + FirstRec.height > SecondRec.y){
        return true;
       }
    else return false;
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
std::vector<Vector2> turtlesPos;
std::vector<Vector2> genericPos;
void readlvlData(Level& lvl){
    std::ifstream in("assets/levels/level1.txt");
    for(std::string line;std::getline(in,line);) lvl.rows.push_back(line);
    for(size_t y=0;y<lvl.rows.size();++y)
        for(size_t x=0;x<lvl.rows[y].size();++x)
        //Making it so that enemies and such are interperated once and then painted as floor.
            switch(lvl.rows[y][x]){
                case('p'): lvl.playerPos = {(float)x, (float)y}; lvl.rows[y][x]='.'; break;
                //Enemies below
                case('t'): lvl.rows[y][x]='.';turtlesPos.emplace_back(Vector2{static_cast<float>(x),static_cast<float>(y)}); break;
                case('e'): lvl.rows[y][x]='.';genericPos.emplace_back(Vector2{static_cast<float>(x),static_cast<float>(y)}); break;
            }
}
struct Enemy{
    float w;
    float h;
    float ID;
    Texture2D spSh;
    Enemy(float w, float h, Texture2D spSh, int ID)
    : w(w), h(h), spSh(spSh), ID(ID) {}
};
Enemy generic(10.0f, 10.0f, tiles.wall, 0);
Enemy TurtleMaster(20.0f, 20.0f, tiles.wall, 1);
void drawLevel(Level& lvl, float s){
    for(size_t y=0;y<lvl.rows.size();++y)
        for(size_t x=0;x<lvl.rows[y].size();++x){
            float px = x * TILE * s;
            float py = y * TILE * s;
            float sz = (TILE * s);
            Rectangle mapTile = {(float)px, (float)py, (float)sz, (float)sz};
            Rectangle srcTile = {0, 0, 16, 16};
            switch(lvl.rows[y][x]){
                case('#'): DrawTexturePro(tiles.wall, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('.'): DrawTexturePro(tiles.floor, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('x'): DrawTexturePro(tiles.wall, srcTile, mapTile, {0,0}, 0, WHITE); break;
                default: break;
            }
        }
}
void loadEnemies(Level& lvl, float s){
    static float sz = (TILE * s);
    for(Vector2 e : genericPos){
        float px = e.x * TILE * s;
        float py = e.y * TILE * s;
        Rectangle mapTile = {(float)px, (float)py, (float)sz, (float)sz};
        spawnLogic({mapTile.x, mapTile.y}, generic.w, generic.h, 10, 0);
    }
    for(Vector2 t : turtlesPos){
        float px = t.x * TILE * s;
        float py = t.y * TILE * s;
        Rectangle mapTile = {(float)px, (float)py, (float)sz, (float)sz};
        spawnLogic({mapTile.x, mapTile.y}, TurtleMaster.w, TurtleMaster.h, 10, 1);
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
        if(currentDir == Direction::Left || currentDir == Direction::DownLeft || currentDir == Direction::UpLeft){
            switch(currentRace){
                case(raceP::spaceLizard): playerTex = SfacingDownLeft.pos; currentDir = Direction::DownLeft; break;
                case(raceP::voidCrawler): playerTex = VfacingDownLeft.pos; currentDir = Direction::DownLeft; break;
                case(raceP::human): playerTex = HfacingDownLeft.pos; currentDir = Direction::DownLeft; break;
                //Add mechasapien
            }
        }
        else{
            switch(currentRace){
                case(raceP::spaceLizard): playerTex = SfacingDownRight.pos; currentDir = Direction::DownRight; break;
                case(raceP::voidCrawler): playerTex = VfacingDownRight.pos; currentDir = Direction::DownRight; break;
                case(raceP::human): playerTex = HfacingDownRight.pos; currentDir = Direction::DownRight; break;
                //Add mechasapien
            }
        }
    }
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        rightZoom = true;
    }
    else{
        rightZoom = false;
    }
    static float w = 5.0f;
    static float h = 5.0f;
    static float speed = 300.0f;
    static float ARCSIZE = 50.0f;
    static float range = 50.0f;
    Rectangle dest; Vector2 origin; float rotation;
    mouseWorld = GetScreenToWorld2D(GetMousePosition(), cam);
    dir = Vector2Normalize(
        Vector2Subtract(mouseWorld, {playerPixCenter.x - w/2, playerPixCenter.y - h/2}) 
    );
    const static float WEAPON_OFFSET = 15.0f;
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
        enemyCollisionCheck();
    }
    if(attacking){
        // Put in an animtion into this function.
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
    float WEAPON_OFFSET = 12.5f * scale;
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
    switch(equipped){
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
    if(rightZoom) cam.zoom = 2 / scale, equipped = blaster;
    else cam.zoom = 1 / scale, equipped = sword;
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
    enemyLogic(dt, lvl, playerPixCenter);
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
                    DrawTexturePro(tiles.wall, srcTile, mapTile, {0, 0}, 0.0f, WHITE);
                }
            }
        }
    }
    EndMode2D();
}
void preLoadTasks(Level& lvl){
    readlvlData(lvl);
    scaleX = GetScreenWidth()/(float)(lvl1.rows[0].size()*TILE);
    scaleY = GetScreenHeight()/(float)(lvl1.rows.size()*TILE);
    scale = (scaleX+scaleY)/2.0f;
    cam.offset = {GetScreenWidth()/2.0f,GetScreenHeight()/2.0f};
    cam.rotation = 0.0f;
    spriteManager();
    loadTileTextures();
    loadEnemies(lvl, scale);
    playerTex = LoadTexture("assets/graphics/void_crawler/void_crawler3.png");
    blasterTex = LoadTexture("assets/graphics/abilities/utilities/equipables/ranged/blaster.png");
    swordTex = LoadTexture("assets/graphics/abilities/utilities/equipables/melee/sword.png");
    SetTextureFilter(playerTex,TEXTURE_FILTER_POINT);
    SetTextureFilter(tiles.wall , TEXTURE_FILTER_POINT);
    SetTextureFilter(tiles.floor, TEXTURE_FILTER_POINT);
    bullets.reserve(1000);
    turtlesPos.reserve(300);
    genericPos.reserve(300);
}
void loadLvl1(){
    static bool loaded=false;
    if(!loaded){
        preLoadTasks(lvl1);
        loaded=true;
    }
    gameLoop(lvl1);
}
void loadLvl2(){
    static bool loaded=false;
    if(!loaded){
        preLoadTasks(lvl2);
        loaded=true;
    }
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