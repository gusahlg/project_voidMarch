#pragma once
// Bellow is a list of functions, classes and variables that all .cpp files may use.
//From main.cpp
void updateHome();
void updateSelect();
class screen {
    public:
        int screenIdentifier = 0;
    void update();
};
extern screen home; 
extern screen select;
extern screen game;
extern bool Hclicked;
extern Color selectC;
//From select.cpp
class selectScr{
    public:
    void selectScrUpdate(); 
};
extern selectScr panel;