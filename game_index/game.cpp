#include "raylib.h"
#include <iostream>
#include "main_select.h"
#include "Sbuttons.hpp"
#include "loadGame.hpp"
#include <vector>
#include <fstream>
const int TILE = 16;
const int SCALE = 4;
void loadLvl1(){
    DrawRectangle(0, 0, 800, 800, PINK);
}
struct Level{
    std::vector<std::string> rows;
    Vector2 playerPos;
};
Level lvl1;
std::ifstream in("assets/levels/level1.txt");

void readlvlData(){
    // Loads game levels.
for(std::string line; std::getline(in, line);){
    lvl1.rows.push_back(line);
}
// Takes the file data and converts it into easy to understand x and y coordinates for the player and then stores int in the lvl1 variable for player position, right?
for(size_t y = 0; y < lvl1.rows.size(); ++y){
    for(size_t x = 0; x < lvl1.rows[y].size(); ++x){
        if(lvl1.rows[y][x] == 'p'){
            lvl1.playerPos = {(float)x, (float)y};
        }
    }
}
}

void drawLevel(const Level& lvl){
    for(size_t y = 0; y < lvl.rows.size(); ++y){
        for(size_t x = 0; x < lvl.rows[y].size(); ++x){
            
        }
    }
}