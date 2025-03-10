#pragma once

#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "Menu.h"
#include "TouchManager.h"

class PauseMenu : public Menu
{
private:
	SDL_Rect rPause;
    static const std::string TOUCH_GAME;
    static const std::string TOUCH_OPTIONS;
    static const std::string TOUCH_MENU;
    static const std::string TOUCH_DESKTOP;
public:
	PauseMenu(void);
	~PauseMenu(void);

	void Update();
	void Draw(SDL_Renderer* rR);

	void enter();
	void escape();

	void updateActiveButton(int iDir);

    void setupPauseMenuTouchAreas();
};

#endif