#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>
#include <limits>
#include <string_view>
extern std::vector<Vector2> turtlesPos;
extern std::vector<Vector2> genericPos;
class Level{
public:
    std::vector<std::string> rows;
    Vector2 playerPos{0,0};
    int ID = 0;
    explicit Level(std::string_view file)
    : path(file) {}
    Level() = default;
    void readlvlData(){
        rows.clear();
        playerPos = {0,0};
        turtlesPos.clear();
        genericPos.clear();
        // Check for smallest filler width in entire file, then remove filler
        // and load each new line into rows.
        int minWidth = std::numeric_limits<int>::max();     
        int maxWidth = std::numeric_limits<int>::max();
        std::ifstream in(path);
        // Might work, need to include for scenarios of hollow maps though.
        bool shouldStop = false;
        for(std::string line; std::getline(in, line);){
            for(int i=0; i < line.size(); ++i){
                if(line[i]=='C') shouldStop = true;
            }
            if(shouldStop) break;
            size_t first = line.find_first_not_of(" -");
            int left = (first == std::string::npos) ? static_cast<int>(line.size()) : static_cast<int>(first);
            size_t last = line.find_last_not_of(" -");
            int right = (last == std::string::npos) ? static_cast<int>(line.size()) : static_cast<int>((line.size() - 1) - last);
            if(left  < minWidth) minWidth = left;   
            if(right < maxWidth) maxWidth = right; 
        }
        in.clear();
        in.seekg(0);
        for(std::string line; std::getline(in, line);){
            const size_t start = std::min<size_t>(minWidth, line.size());
            size_t end = line.size() - std::min<size_t>(maxWidth, line.size());
            if(end < start) end = start;
            // Cut off excess filler with substr:
            rows.emplace_back(line.substr(start, end - start));
        }
        // Searches for enemies etc.
        for(size_t y = 0; y < rows.size(); ++y){
            for(size_t x = 0; x < rows[y].size(); ++x){
                switch(rows[y][x]){
                    case 'p': playerPos = {(float)x, (float)y }; rows[y][x] = '.'; break;
                    case 't': turtlesPos.emplace_back(Vector2{(float)x, (float)y}); rows[y][x] = '.'; break;
                    case 'e': genericPos.emplace_back(Vector2{(float)x, (float)y}); rows[y][x] = '.'; break;
                    default: break;
                }
            }
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
extern float Ox;
extern float Oy;
extern bool attacking;
extern Texture2D swordTex;
extern float PLAYERWIDTH;
extern const float SQRT2;
extern float PLAYERHEIGHT;
void updateRoll(Level& lvl, float dt);
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
    DownRight,
    _COUNT
};
extern Direction currentDir;
bool R2CollCheck(Rectangle FirstRec, Rectangle SecondRec);