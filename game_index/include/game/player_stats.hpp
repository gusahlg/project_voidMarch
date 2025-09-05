// for game.cpp and sprites.
#pragma once
#include <cstdint>
#include <vector>
// Output from the select that can then be used in the game:
enum class Prof : std::uint8_t{Necromancer, Craftsman, Wizard, Dualist};
extern Prof currentProf;
enum class Race : std::uint8_t{Spacelizard, Voidcrawler, Mecha_sapien, Human};
extern Race currentRace;
// Vision is to have all walk and roll animations tied to the player struct,
// You can easily load and unload the players textures.
enum class WalkDir : std::uint8_t{up, upLeft, upRight, down, downLeft, downRight, right, left};
struct PlayerTexManager{
    std::array<Texture2D, 8> walkTextures;
    Texture2D rollTex; 
    PlayerTexManager(Texture2D& up, Texture2D& upLeft, Texture2D& upRight,
                     Texture2D& down, Texture2D& downLeft, Texture2D& downRight,
                     Texture2D& right, Texture2D& left, Texture2D& rollTex){
        walkTextures[1]=up; walkTextures[2]=upLeft; walkTextures[3]=upRight;
        walkTextures[4]=down; walkTextures[5]=downLeft; walkTextures[6]=downRight;
        walkTextures[7]=up; walkTextures[8]=upLeft;
        this->rollTex=rollTex;
    }
    ~PlayerTexManager(){
        for(Texture2D& tex : walkTextures){
            UnloadTexture(tex);
        }
    }
};
extern PlayerTexManager playerTex;
