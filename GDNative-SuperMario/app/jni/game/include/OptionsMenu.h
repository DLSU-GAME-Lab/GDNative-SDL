#pragma once

#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "Menu.h"
#include "TouchManager.h"

class OptionsMenu : public Menu
{
private:
	SDL_Rect rRect;
	SDL_Rect rSetKeyRect;
	SDL_Rect rVolumeBG;
	SDL_Rect rVolume;

	bool inSetKey, resetSetKey;

	bool escapeToMainMenu;

    // Touch area IDs for options
    static const std::string TOUCH_DPAD_JOYSTICK;
    static const std::string TOUCH_BACKWARDS;
    static const std::string TOUCH_PAUSE;
    static const std::string TOUCH_MENU;

    //to check if touch areas were initialized or not
    bool touchAreasInitialized = false;

    //to show if dpad or joystick
    std::string strDpadOrJoystick = "";
public:
	OptionsMenu(void);
	~OptionsMenu(void);

	void Update();
	void Draw(SDL_Renderer* rR);

	void enter();
	void escape();
	void setKey(int keyID);

	void updateActiveButton(int iDir);

	void updateVolumeRect();

	void setEscapeToMainMenu(bool escapeToMainMenu);

    void setupOptionsMenuTouchAreas();
    void clearTouchAreas();
    void changeDpadOrJoystick();
};

#endif