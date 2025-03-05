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

    // Touch area IDs for world selection
    static const std::string TOUCH_WORLD_PREFIX;
    static const std::string TOUCH_LEVEL_PREFIX;
    static const std::string TOUCH_WORLD_START;
    static const std::string TOUCH_WORLD_BACK;

public:
    MainMenu(void);
    ~MainMenu(void);

    void Update();
    void Draw(SDL_Renderer* rR);

    void enter();
    void escape();

    void updateActiveButton(int iDir);

    // Touch control methods
    void setupMenuTouchAreas();
    void setupWorldSelectionTouchAreas();
    void clearWorldSelectionTouchAreas();
};

#endif