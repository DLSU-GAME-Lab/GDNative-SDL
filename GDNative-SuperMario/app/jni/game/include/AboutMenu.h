#pragma once

#ifndef ABOUTMENU_H
#define ABOUTMENU_H

#include "Menu.h"
#include "TouchManager.h"

class AboutMenu : public Menu
{
private:
	unsigned int iTime;

	int cR, cG, cB, nR, nG, nB;
	int colorStepID, iColorID;

	// ----- true = RIGHT, false = LEFT
	bool moveDirection;

	int iNumOfUnits;

    // Touch area IDs for return to menu
    static const std::string TOUCH_MENU;

    //to check if touch areas were initialized or not
    bool touchAreasInitialized = false;
public:
	AboutMenu(void);
	~AboutMenu(void);

	void Update();
	void Draw(SDL_Renderer* rR);

	void enter();

	void launch();
	void reset();

	void nextColor();
	int getColorStep(int iOld, int iNew);

	void setBackgroundColor(SDL_Renderer* rR);
	void updateTime();

    // Touch control methods
    void setupMenuTouchArea();
    void setupWorldSelectionTouchAreas();
};

#endif