#include <raylib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <raymath.h>
#include <cstdint>
#include <string_view>
#include "../include/game/loadGame.hpp"
#include "../include/game/player_stats.hpp"
#include "../include/game/ability_attributes.hpp"
#include "../include/game/enemy_data.hpp"
// Everything weapon/inventory related.
#include "../game_logic/inventory/weapon.hpp"
#include "../game_logic/inventory/melee_bindings.hpp"
#include "../game_logic/inventory/sector_hit.hpp"
// Essential systems used for scaling and communicating constants.
#include "../include/global/constants.hpp"
#include "../include/global/scale_utils.hpp"
// Gives access to player data.
#include "../include/data/stats/player.hpp"
// Saving and loading world data.
#include "../include/data/stats/world.hpp"
// Weapon definitions.
#include "items/items.hpp"
void bindEnemyAdapter();
struct TileSet{
    Texture2D WallUp;
    Texture2D WallDown;
    Texture2D WallUpLeft;
    Texture2D WallUpRight;
    Texture2D WallDownLeft;
    Texture2D WallDownRight;
    Texture2D WallLeft;
    Texture2D WallRight;
    Texture2D WallFull;
    Texture2D floor;
    //Add more if needed.
    TileSet(Texture2D WallUp, Texture2D WallDown, Texture2D WallUpLeft, Texture2D WallUpRight, Texture2D WallDownLeft, 
            Texture2D WallDownRight, Texture2D WallLeft, Texture2D WallRight, Texture2D WallFull, Texture2D floor)
    : WallUp(WallUp), WallDown(WallDown), WallUpLeft(WallUpLeft), WallUpRight(WallUpRight), WallDownLeft(WallDownLeft), 
      WallDownRight(WallDownRight), WallLeft(WallLeft), WallRight(WallRight), WallFull(WallFull), floor(floor) {}
};
TileSet tiles{/*wall =*/ {}, /*floor =*/ {}, {}, {}, {}, {}, {}, {}, {}, {}};
bool attacking = false;
Vector2 mouseWorld;
Vector2 playerPixCenter;
Vector2 dir;
float PLAYERWIDTH = 0.9;
float PLAYERHEIGHT = 0.4;
Vector2 spawnPos;
constexpr float WALK_SPEED = 5.0f;
constexpr float SQRT2 = 0.70710678119;
Direction currentDir;
constexpr float STEP_DELAY = 0.005f;
float stepTimer = 0.0f;
void movementEventHandler(Level& lvl, float);
// sprite-sheet data
int PLAYER_FRAMES = 3;
const float ANIM_SPEED = 0.12f;
int currentFrame = 0;
float animTimer = 0.0f;
Camera2D cam{};
void loadTileTextures(){
    const std::vector<std::pair<const char*, Texture2D*>> todo = {
        {"assets/graphics/level_graphics/tiles/walls/Top_Wall.png",         &tiles.WallUp},
        {"assets/graphics/level_graphics/tiles/walls/Bottom_Wall.png",      &tiles.WallDown},
        {"assets/graphics/level_graphics/tiles/walls/Top_Left_Wall.png",    &tiles.WallUpLeft},
        {"assets/graphics/level_graphics/tiles/walls/Top_Right_Wall.png",   &tiles.WallUpRight},
        {"assets/graphics/level_graphics/tiles/walls/Bottom_Left_Wall.png", &tiles.WallDownLeft},
        {"assets/graphics/level_graphics/tiles/walls/Bottom_Right_Wall.png",&tiles.WallDownRight},
        {"assets/graphics/level_graphics/tiles/walls/Left_Wall.png",        &tiles.WallLeft},
        {"assets/graphics/level_graphics/tiles/walls/Right_Wall.png",       &tiles.WallRight},
        {"assets/graphics/level_graphics/tiles/walls/Full_Blue_Wall.png",   &tiles.WallFull},
        {"assets/graphics/level_graphics/tiles/stdbackground.png",          &tiles.floor}
        // Add more textures here and in struct.
    };
    for (auto [file, out] : todo){
        *out = LoadTexture(file);
        SetTextureFilter(*out, TEXTURE_FILTER_POINT);
    }
}
bool shouldLevelProgress(Level& lvl){
    int py = (int)lvl.playerPos.y;
    if(py < 0 || py >= (int)lvl.rows.size()) return true;
    int px = (int)lvl.playerPos.x;
    if(px < 0 || px >= (int)lvl.rows[py].size()) return true;
    return (lvl.rows[py][px] == 'g');
}
bool R2CollCheck(Rectangle FirstRec, Rectangle SecondRec){
    if(FirstRec.x < SecondRec.x + SecondRec.width &&
       FirstRec.x + FirstRec.width > SecondRec.x &&
       FirstRec.y < SecondRec.y + SecondRec.height &&
       FirstRec.y + FirstRec.height > SecondRec.y) return true;
    else return false;
}
bool isWall(float cx, float cy, Level& lvl){
    if(cy < 0 || cy >= (int)lvl.rows.size()) return true;
    else if(cx < 0 || cx >= (int)lvl.rows[cy].size()) return true;
    switch(lvl.rows[cy][cx]){
        case '#': return true; break;
        case '1': return true; break;
        case '2': return true; break;
        case '3': return true; break;
        case '4': return true; break;
        case '5': return true; break;
        case '6': return true; break;
        case '7': return true; break; 
        case '8': return true; break;
        default: return false; break;
    }
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
void drawLevel(const Level& lvl){

    for(size_t y=0;y<lvl.rows.size();++y)
        for(size_t x=0;x<lvl.rows[y].size();++x){
            int px = ScaleUtils::toPx(x);
            int py = ScaleUtils::toPx(y);
            Rectangle mapTile = {(float)px, (float)py, (float)TILE_SIZE, (float)TILE_SIZE};
            Rectangle srcTile = {0, 0, 16, 16};
            switch(lvl.rows[y][x]){
                case('1'): DrawTexturePro(tiles.WallUp, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('2'): DrawTexturePro(tiles.WallUpLeft, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('3'): DrawTexturePro(tiles.WallUpRight, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('4'): DrawTexturePro(tiles.WallDownLeft, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('5'): DrawTexturePro(tiles.WallDownRight, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('6'): DrawTexturePro(tiles.WallLeft, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('7'): DrawTexturePro(tiles.WallRight, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('8'): DrawTexturePro(tiles.WallDown, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('#'): DrawTexturePro(tiles.WallFull, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('.'): DrawTexturePro(tiles.floor, srcTile, mapTile, {0,0}, 0, WHITE); break;
                case('x'): DrawTexturePro(tiles.floor, srcTile, mapTile, {0,0}, 0, WHITE); break;
                default: break;
            }
        }
}
void loadEnemies(Level& lvl){
    for(Vector2 e : genericPos){
        Vector2 pos = {ScaleUtils::toPx(e.x), ScaleUtils::toPx(e.y)};
        spawnLogic(pos, 10, 0);
    }
    for(Vector2 t : turtlesPos){
        Vector2 pos = {ScaleUtils::toPx(t.x), ScaleUtils::toPx(t.y)};
        spawnLogic(pos, 10, 1);
    }
}
bool rollWalkSwitch = false;
float pPixX;
float pPixY;
bool projActive;
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
    }
}
/* Checks for if there's any attack input and if so executes
appropriate actions */
void attackInputHandler(Level& lvl, float dt){
    // MELEE on LEFT click — hit once on click, then animate
    if(!IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Weapon::sword.attackReady(dt)){
        item_sys::start_melee_swing(playerPixCenter, mouseWorld,
                                    Weapon::sword.range(), Weapon::sword.arcDegrees());
        item_sys::resolve_melee_hits(Weapon::sword.damage());  // <-- hit once now
        Weapon::sword.beginAttackAnim();
        Weapon::equipped = Weapon::WeaponSwitch::meleeToggle;
    }
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Weapon::blaster.attackReady(dt)){
        Vector2 dir = Vector2Subtract(mouseWorld, playerPixCenter);
        float len2 = dir.x*dir.x + dir.y*dir.y;
        if(len2 > 1e-8f){
            float inv = 1.0f / sqrtf(len2);
            dir = {dir.x*inv, dir.y*inv};
        } 
        else{
            dir = {1.f, 0.f};
        }
        const float projW = 5.f, projH = 5.f, projSpeed = 1000.f;
        spawnProjectile(playerPixCenter, dir, projW, projH, projSpeed,
                        /*enemyOwner*/ false, Weapon::blaster.damage()); // <-- damage param (step 3)

        Weapon::blaster.beginAttackAnim();
        Weapon::equipped = Weapon::WeaponSwitch::rangedToggle;
    }
}
void updateJson(float dt, Level& lvl){
    static float Delay = 10.f;
    Delay -= dt;
    if(Delay > 0) return;
    gWorld.saveWorldData(lvl.playerPos.x,lvl.playerPos.y,lvl.ID);
    Delay = 10.f;
}
int pSizeW = 18;
int pSizeH = 25;
Rectangle src;
Rectangle dst;
Rectangle playerRect;
void gameLoop(Level& lvl){
    if(shouldLevelProgress(lvl)){
        gWorld.saveWorldData(lvl.playerPos.x, lvl.playerPos.y, ++lvl.ID);
        lvl.readlvlData();
    }
    if(gPlayer.isDead()) return;// Do something cool
    mouseWorld = GetScreenToWorld2D(GetMousePosition(), cam);
    float dt = GetFrameTime();
    playerPixCenter = {(float)ScaleUtils::toPx(lvl.playerPos.x) + pSizeW/2,
                       (float)ScaleUtils::toPx(lvl.playerPos.y) + pSizeH/2};
    cam.target = playerPixCenter;
    cam.target.x = floorf(cam.target.x + 0.5f);
    cam.target.y = floorf(cam.target.y + 0.5f);
    cam.offset.x = floorf(cam.offset.x + 0.5f);
    cam.offset.y = floorf(cam.offset.y + 0.5f);
    ClearBackground(BLACK);
    BeginMode2D(cam);

    pPixX = ScaleUtils::toPx(lvl.playerPos.x);
    pPixY = ScaleUtils::toPx(lvl.playerPos.y);
    src = {currentFrame * (float)pSizeW, 0.0f, (float)pSizeW, (float)pSizeH};
    dst = {pPixX, pPixY, (float)pSizeW, (float)pSizeH};
    // Used for collision and such:
    playerRect = {pPixX, pPixY, (float)pSizeW, (float)pSizeH};
    drawLevel(lvl);
    enemyLogic(dt, lvl, playerPixCenter);
    inputEventHandler(lvl, dt);
    attackInputHandler(lvl, dt);
    const Texture2D& ptex = rolling
        ? PlayerTexManager::instance().roll(currentDir)
        : PlayerTexManager::instance().walk(currentDir);
    DrawTexturePro(ptex, src, dst, {0,0}, 0.0f, WHITE);
    if(currentDir == Direction::UpLeft || currentDir == Direction::UpRight || currentDir == Direction::Up){
        DrawTexturePro(ptex, src, dst, {0,0}, 0.0f, WHITE);
    }
    if(wallBellow(lvl.playerPos.x, lvl.playerPos.y, lvl)){
        Rectangle srcTile = { 0, 0, 16, 16 };
        for(int y = 0; y < (int)lvl.rows.size(); ++y){
            for(int x = 0; x < (int)lvl.rows[y].size(); ++x){
                Rectangle mapTile = {ScaleUtils::toPx(x), ScaleUtils::toPx(y), TILE_SIZE, TILE_SIZE};
                switch(lvl.rows[y][x]){
                    case '#': DrawTexturePro(tiles.WallFull, srcTile, mapTile, {0, 0}, 0.0f, WHITE); break;
                    case '1': DrawTexturePro(tiles.WallUp, srcTile, mapTile, {0, 0}, 0.0f, WHITE); break;
                    case '2': DrawTexturePro(tiles.WallUpLeft, srcTile, mapTile, {0, 0}, 0.0f, WHITE); break;
                    case '3': DrawTexturePro(tiles.WallUpRight, srcTile, mapTile, {0, 0}, 0.0f, WHITE); break;
                    case '4': DrawTexturePro(tiles.WallDownLeft, srcTile, mapTile, {0, 0}, 0.0f, WHITE); break;
                    case '5': DrawTexturePro(tiles.WallDownRight, srcTile, mapTile, {0, 0}, 0.0f, WHITE); break;
                    case '6': DrawTexturePro(tiles.WallLeft, srcTile, mapTile, {0, 0}, 0.0f, WHITE); break;
                    case '7': DrawTexturePro(tiles.WallRight, srcTile, mapTile, {0, 0}, 0.0f, WHITE); break;
                    case '8': DrawTexturePro(tiles.WallDown, srcTile, mapTile, {0, 0}, 0.0f, WHITE); break;
                }
            }
        }
    }
    Weapon::sword.tickAnim(dt);
    Weapon::blaster.tickAnim(dt);
    updateProjectiles(lvl, dt);
    drawProjectiles();
    // Draw equipped weapon (melee = no rotation; ranged = rotates in its override)
    if(Weapon::equipped == Weapon::WeaponSwitch::meleeToggle){
        Weapon::sword.draw(playerPixCenter, mouseWorld);
    } 
    else{
        Weapon::blaster.draw(playerPixCenter, mouseWorld);
    }
    updateRangedAttack(playerPixCenter, mouseWorld, 0, 0, 0, dt, lvl);
    EndMode2D();
    updateJson(dt, lvl);
}
Level lvl1("assets/levels/level1.txt");
Level lvl2("assets/levels/level2.txt");
void preLoadTasks(Level& lvl){
    lvl1.ID = 1;
    lvl2.ID = 2;
    lvl.readlvlData();
    Weapon::loadTextures();
    cam.offset = {GetScreenWidth()/2.0f,GetScreenHeight()/2.0f};
    cam.zoom = 1.f;
    cam.rotation = 0.0f;
    loadTileTextures();
    loadEnemies(lvl);
    PlayerTexManager::instance().loadWalkFor(currentRace);
    PlayerTexManager::instance().loadRollFor(currentRace);
    bullets.reserve(50);
    turtlesPos.reserve(50);
    genericPos.reserve(50);
    bindEnemyAdapter();
    gWorld.saveWorldData(lvl.playerPos.x,lvl.playerPos.y,lvl.ID);
}
void loadLvl(){
    static bool loaded=false;
    Level& lvl = (gWorld.currentLevel() == 1) ? lvl1 : lvl2;
    if(!loaded){
        preLoadTasks(lvl);
        loaded=true;
    }
    gameLoop(lvl);
}
void gameStateEventHandler(){
    loadLvl();
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
    float newX = lvl.playerPos.x + x * WALK_SPEED * dt;
    float newY = lvl.playerPos.y + y * WALK_SPEED * dt;
    if(!collisionRect(newX, newY, PLAYERWIDTH, PLAYERHEIGHT, lvl)){
        lvl.playerPos.x = newX;
        lvl.playerPos.y = newY;
    }
}