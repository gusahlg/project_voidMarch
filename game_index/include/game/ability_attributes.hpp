#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>
#include <limits>
#include <cmath> 
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
struct enemy{
    int frames;
    int currentFrame = 0;
    float animDelay;
    float animTimer = 0.f;
    Rectangle Hbox;
    int MAXHP;      // Maximum HP.
    int HP;         // Actual HP, mutable.
    float cooldown; // Attack cooldown duration
    int range;    // Detection range
    Texture2D tex;  // Enemy texture
    enum dir : std::uint8_t{Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight};
    dir currentDir = Up;
    enum state : std::uint8_t{Idle, Walking, Jumping};
    state currentState = Idle;
    enum class Type : std::uint8_t{generic, TurtleMaster, Bob}; //Expand for more variations
    Type kind = Type::generic;
    // Bellow depends on type.
    float speed{};
    void update(Vector2 playerPixCenter){
        // System below is for movement and updating state.
        Vector2 Emid = {Hbox.x + Hbox.width/2.0f, Hbox.y + Hbox.height/2.0f};
        bool inRangeX = std::fabs(Emid.x - playerPixCenter.x) <= range;
        bool inRangeY = std::fabs(Emid.y - playerPixCenter.y) <= range;
        float distX = Emid.x < playerPixCenter.x ? playerPixCenter.x - Emid.x : Emid.x - playerPixCenter.x;
        float distY = Emid.y < playerPixCenter.y ? playerPixCenter.y - Emid.y : Emid.y - playerPixCenter.y;
        float dx = std::fabs(playerPixCenter.x - Emid.x);
        float dy = std::fabs(playerPixCenter.y - Emid.y);
        // Add in randomness and such for good pathfinding, current system is temporary
        if(inRangeX && inRangeY){
            currentState = state::Walking;
        }
        else currentState = state::Idle;
        // Check if diagonals are faster, then left/right, up/down.
        if(dx >= 1.0f && dy >= 1.0f){ 
            bool right = Emid.x < playerPixCenter.x;
            bool down  = Emid.y < playerPixCenter.y;
            currentDir = right
                        ? (down ? DownRight : UpRight)
                        : (down ? DownLeft  : UpLeft);
        }
        else if(dx > dy){  
            currentDir = (Emid.x < playerPixCenter.x) ? Right : Left;
        }
        else if(dy > 0.0f){
            currentDir = (Emid.y < playerPixCenter.y) ? Down : Up;
        }
        else{
            currentState = Idle;
        }
    }
    // Determine if damage should be dealt to player.
    bool playerInRange(Vector2 playerPixCenter){
        Vector2 Emid = {Hbox.x + Hbox.width/2.0f, Hbox.y + Hbox.height/2.0f};
        bool inRangeX = std::fabs(Emid.x - playerPixCenter.x) <= range;
        bool inRangeY = std::fabs(Emid.y - playerPixCenter.y) <= range;
        if(inRangeX && inRangeY) return true;
        else return false;
    }
    void draw(){
        switch(currentState){
            case state::Walking:
                animTimer += GetFrameTime();
                if(animTimer > animDelay){
                    currentFrame = (currentFrame + 1) % frames;
                    animTimer = 0.f;
                }
                break;
            case state::Idle:
                animTimer = 0.0f;
                currentFrame = 0;
                break;
            case state::Jumping:
                //Cool things
                break;
        }
        float frameW = (float)tex.width / frames; 
        Rectangle src = {currentFrame * frameW, 0, frameW, (float)tex.height};
        DrawTexturePro(tex, src, Hbox, {0,0}, 0, WHITE);
    }
    enemy(Vector2 pos, int hp, Type t)
    : Hbox{}, MAXHP(hp), HP(hp), kind(t)
{
    const size_t idx = static_cast<size_t>(kind);
    // Constant LUT 
    static constexpr float   speedLUT      [] = {90.f, 20.f, 50.f};
    static constexpr float   delayLUT      [] = {30.f, 40.f, 50.f};
    static constexpr float   rangeTilesLUT [] = {5 * 16, 10 * 16, 3 * 16};
    static constexpr float   animDelayLUT  [] = {0.1f, 0.25f, 5.f};
    static constexpr int     framesLUT     [] = {3, 4, 5};
    //Determined values from tables above
    speed     = speedLUT[idx]; //Delta time is updated in the determineState function.
    cooldown  = delayLUT[idx];
    range     = rangeTilesLUT[idx];
    animDelay = animDelayLUT[idx];
    frames    = framesLUT[idx];

    // Texture determination:
    switch(idx){
        case 0: tex = LoadTexture("assets/graphics/enemies/Turtlemaster.png"); break;
        case 1: tex = LoadTexture("assets/graphics/enemies/Turtlemaster.png"); break;
        case 2: tex = LoadTexture("assets/graphics/enemies/Turtlemaster.png"); break;
    }
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);

    // Hitbox calc
    float frameW = (float)tex.width / frames;
    Hbox = Rectangle{ pos.x, pos.y, frameW, (float)tex.height};
}
};
extern bool rolling;
extern bool moving;
extern float Ox;
extern float Oy;
extern float PLAYERWIDTH;
extern const float SQRT2;
extern float PLAYERHEIGHT;
void updateRoll(Level& lvl, float dt);
bool collisionRect(float cx, float cy, float cw, float ch, Level& lvl);
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