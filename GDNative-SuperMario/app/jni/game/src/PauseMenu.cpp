#include "PauseMenu.h"
#include "CFG.h"
#include "GDCore.h"

/* ******************************************** */
const std::string PauseMenu::TOUCH_GAME = "pause_menu_game";
const std::string PauseMenu::TOUCH_OPTIONS = "pause_menu_options";
const std::string PauseMenu::TOUCH_MENU = "pause_menu_menu";
const std::string PauseMenu::TOUCH_DESKTOP = "pause_menu_desktop";

PauseMenu::PauseMenu(void) {
	rPause.x = 220;
	rPause.y = 140;
	rPause.w = 360;
	rPause.h = 142;

	this->lMO.push_back(new MenuOption("RESUME", 0, 156));
	this->lMO.push_back(new MenuOption("OPTIONS", 0, 180));

	this->lMO.push_back(new MenuOption("QUIT TO MENU", 0, 228));
	this->lMO.push_back(new MenuOption("QUIT TO DESKTOP", 0, 252));

	this->numOfMenuOptions = lMO.size();

    this->setupPauseMenuTouchAreas();
}

PauseMenu::~PauseMenu(void) {
    TouchManager::getInstance()->removeTouchArea(TOUCH_MENU);
    TouchManager::getInstance()->removeTouchArea(TOUCH_GAME);
    TouchManager::getInstance()->removeTouchArea(TOUCH_OPTIONS);
    TouchManager::getInstance()->removeTouchArea(TOUCH_DESKTOP);
}

/* ******************************************** */

void PauseMenu::Update() {

}

void PauseMenu::Draw(SDL_Renderer* rR) {
	SDL_SetRenderDrawBlendMode(rR, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(rR, 4, 4, 4, 235);
	SDL_RenderFillRect(rR, &rPause);
	SDL_SetRenderDrawColor(rR, 255, 255, 255, 255);
	rPause.x += 1;
	rPause.y += 1;
	rPause.h -= 2;
	rPause.w -= 2;
	SDL_RenderDrawRect(rR, &rPause);
	rPause.x -= 1;
	rPause.y -= 1;
	rPause.h += 2;
	rPause.w += 2;

	for(unsigned int i = 0; i < lMO.size(); i++) {
		if(i == activeMenuOption) {
			CCFG::getText()->DrawCenterX(rR, lMO[i]->getText(), lMO[i]->getYPos(), 16, 255, 255, 255);
		} else {
			CCFG::getText()->DrawCenterX(rR, lMO[i]->getText(), lMO[i]->getYPos(), 16, 90, 90, 90);
		}
	}

	SDL_SetRenderDrawBlendMode(rR, SDL_BLENDMODE_NONE);
	GDCore::getMap()->setBackgroundColor(rR);

    TouchManager::getInstance()->drawTouchAreas(rR);
}

/* ******************************************** */

void PauseMenu::enter() {
    switch(activeMenuOption) {
        case 0:
            CCFG::getMM()->setViewID(CCFG::getMM()->eGame);
            CCFG::getMusic()->PlayMusic();
            // Switch back to gameplay scene
            TouchManager::getInstance()->setActiveScene("gameplay");
            break;
        case 1:
            CCFG::getMM()->getOptions()->setEscapeToMainMenu(false);
            CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->eOptions);
            CCFG::getMM()->getOptions()->updateVolumeRect();
            CCFG::getMM()->setViewID(CCFG::getMM()->eOptions);
            break;
        case 2:
            GDCore::getMap()->resetGameData();
            CCFG::getMM()->setViewID(CCFG::getMM()->eMainMenu);
            // Switch to main menu scene
            TouchManager::getInstance()->setActiveScene("main_menu");
            break;
        case 3:
            GDCore::quitGame = true;
            break;
    }
}

void PauseMenu::escape() {
    CCFG::getMM()->setViewID(CCFG::getMM()->eGame);
    CCFG::getMusic()->PauseMusic();
    // Switch back to gameplay scene
    TouchManager::getInstance()->setActiveScene("gameplay");
}
void PauseMenu::updateActiveButton(int iDir) {
	Menu::updateActiveButton(iDir);
}

void PauseMenu::setupPauseMenuTouchAreas() {
    const int TOUCH_WIDTH = 220;   // Wide enough for the text
    const int TOUCH_HEIGHT = 40;   // Tall enough to touch easily

    // Center the touch areas horizontally
    int centerX = 400; // Center of the screen horizontally

    // Define the four touch areas
    // RESUME
    SDL_Rect resumeBounds = {
            centerX - TOUCH_WIDTH/2,  // Center horizontally
            lMO[0]->getYPos() - 15,   // Positioned based on text Y position
            TOUCH_WIDTH,
            TOUCH_HEIGHT
    };

    // OPTIONS
    SDL_Rect optionsBounds = {
            centerX - TOUCH_WIDTH/2,
            lMO[1]->getYPos() - 15,
            TOUCH_WIDTH,
            TOUCH_HEIGHT
    };

    // QUIT TO MENU
    SDL_Rect menuBounds = {
            centerX - TOUCH_WIDTH/2,
            lMO[2]->getYPos() - 15,
            TOUCH_WIDTH,
            TOUCH_HEIGHT
    };

    // QUIT TO DESKTOP
    SDL_Rect desktopBounds = {
            centerX - TOUCH_WIDTH/2,
            lMO[3]->getYPos() - 15,
            TOUCH_WIDTH,
            TOUCH_HEIGHT
    };

    // Create touch areas with callbacks and assign to "pause_menu" scene
    TouchManager::getInstance()->addTouchArea(TOUCH_GAME, resumeBounds,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      activeMenuOption = 0;
                                                      enter();
                                                  }
                                              },
                                              "pause_menu");

    TouchManager::getInstance()->addTouchArea(TOUCH_OPTIONS, optionsBounds,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      activeMenuOption = 1;
                                                      enter();
                                                  }
                                              },
                                              "pause_menu");

    TouchManager::getInstance()->addTouchArea(TOUCH_MENU, menuBounds,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      activeMenuOption = 2;
                                                      enter();
                                                  }
                                              },
                                              "pause_menu");

    // Add new touch area for QUIT TO DESKTOP
    TouchManager::getInstance()->addTouchArea(TOUCH_DESKTOP, desktopBounds,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      activeMenuOption = 3;
                                                      enter();
                                                  }
                                              },
                                              "pause_menu");

    // Set semi-transparent colors for the touch areas
    SDL_Color normalColor = {200, 200, 200, 60};   // Light gray, semi-transparent
    SDL_Color pressedColor = {255, 255, 255, 100}; // White, more visible when pressed
    SDL_Color borderColor = {255, 255, 255, 80};   // White border, semi-transparent

    // Apply colors to all menu touch areas
    for (const auto& id : {TOUCH_GAME, TOUCH_OPTIONS, TOUCH_MENU, TOUCH_DESKTOP}) {
        if (TouchArea* area = TouchManager::getInstance()->getTouchArea(id)) {
            area->normalColor = normalColor;
            area->pressedColor = pressedColor;
            area->borderColor = borderColor;
        }
    }

    // Set pause_menu as the active scene when this is called
    TouchManager::getInstance()->setActiveScene("pause_menu");
}