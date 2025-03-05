#pragma once

#ifndef MAINMENU_H
#define MAINMENU_H

#include "Menu.h"
#include "TouchManager.h"

class MainMenu : public Menu
{
private:
    bool selectWorld;
    int activeWorldID, activeSecondWorldID;

    SDL_Rect rSelectWorld;

    // Touch area IDs for menu options
    static const std::string TOUCH_1PLAYER;
    static const std::string TOUCH_OPTIONS;
    static const std::string TOUCH_ABOUT;

public:
    MainMenu(void);
    ~MainMenu(void);

    void Update();
    void Draw(SDL_Renderer* rR);

    void enter();
    void escape();

    void updateActiveButton(int iDir);

    // New function to create touch areas for menu options
    void setupMenuTouchAreas();

    // Helper function to select a menu option by index
    void selectMenuOption(int optionIndex);
};

#endif