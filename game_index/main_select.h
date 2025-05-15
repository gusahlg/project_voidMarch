#pragma once
// Bellow is a list of functions, classes and variables that all .cpp files may use.

//From main.cpp
void updateHome();
void updateSelect();
void updateGame();
class screen {
public:
    int screenIdentifier;
    void update();
};
screen home; 
screen select;
screen game;

//From select.cpp
void Coolhet();

