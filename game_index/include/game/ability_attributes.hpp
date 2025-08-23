#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include <cstdint>
#include <string>
#include "player_stats.hpp"
#include <fstream>
#include <string_view>
extern std::vector<Vector2> turtlesPos;
extern std::vector<Vector2> genericPos;
class Level {
public:
    std::vector<std::string> rows;
    Vector2 playerPos{0,0};
    int ID = 0;
    explicit Level(std::string_view file)
    : path(file) {}
    Level() = default;
    /*
    This is the old code, compare what old/new code does to debug.
    void readlvlData(Level& lvl){
        for(std::string line;std::getline(in,line);) lvl.rows.push_back(line);
        for(size_t y=0;y<rows.size();++y)
            for(size_t x=0;x<lvl.rows[y].size();++x)
            //Making it so that enemies and such are interperated once and then painted as floor.
                switch(lvl.rows[y][x]){
                    case('p'): lvl.playerPos = {(float)x, (float)y}; lvl.rows[y][x]='.'; break;
                    //Enemies below
                    case('t'): lvl.rows[y][x] = '.'; turtlesPos.emplace_back(Vector2{static_cast<float>(x),static_cast<float>(y)}); break;
                    case('e'): lvl.rows[y][x] = '.'; genericPos.emplace_back(Vector2{static_cast<float>(x),static_cast<float>(y)}); break;
                }
    }
                */
    // Read the level text, capture p/e/t markers, convert them to '.' in the tilemap.
    inline void readlvlData(){
        rows.clear();
        playerPos = {0,0};
        turtlesPos.clear();
        genericPos.clear();
        std::ifstream in(path);
        std::string line;
        size_t y = 0;
        while(std::getline(in, line)){
            if(line.find('C') != std::string::npos) break;
            std::string row; row.reserve(line.size());
            for(size_t x = 0; x < line.size(); ++x){
                char c = line[x];
                switch(c){
                    case 'p': playerPos = {float(x), float(y)}; row.push_back('.'); break;
                    case 't': turtlesPos.push_back(Vector2{float(x), float(y)}); row.push_back('.'); break;
                    case 'e': genericPos.push_back(Vector2{float(x), float(y)}); row.push_back('.'); break;
                }
            }
            rows.push_back(std::move(row));
            ++y;
        }
    }
private:
    std::string path;
};
struct projectile {
    Vector2 pos;
    Vector2 vel;
    float w, h;
    bool alive;
    bool enemyOwner; // If true, it deals damage to player and has no collision for enemies.
    projectile(Vector2 p, Vector2 v, float w_, float h_, bool enemyOwner)
      : pos(p), vel(v), w(w_), h(h_), alive(true), enemyOwner(enemyOwner) {}
};
extern std::vector<projectile> bullets;
extern bool rolling;
extern bool moving;
void defineDamageArea(Vector2 centerpos, float radius, Vector2 dir, float ARCSIZE);
extern float Ox;
extern float Oy;
extern bool attacking;
extern Texture2D swordTex;
extern float PLAYERWIDTH;
extern const float SQRT2;
extern float PLAYERHEIGHT;
void loadRollTex();
void updateMeleeAttack(Vector2 pos, Vector2 dir, float ARCSIZE, float radius, Level& lvl, Rectangle dest, Vector2 origin, float rotation);
void updateRoll(Level& lvl, float dt);
extern Texture2D playerTex;
bool collisionRect(float cx, float cy, float cw, float ch, Level& lvl);
void updateRangedAttack(Vector2 pos, Vector2 dir, float projW, float projH, float projSpeed, float dt, Level& lvl);
void spawnProjectile(Vector2 startpos, Vector2 dir, float w, float h, float speed, bool enemyOwner);
extern bool projActive;
enum Direction : std::uint8_t{
    Up,
    Down,
    Left,
    Right,
    UpLeft,
    UpRight,
    DownLeft,
    DownRight
};
extern Direction currentDir;
enum weapon : std::uint8_t{
    sword,
    blaster
};
extern weapon equipped;
bool R2CollCheck(Rectangle FirstRec, Rectangle SecondRec);