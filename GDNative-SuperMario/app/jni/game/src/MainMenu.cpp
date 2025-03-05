#include "MainMenu.h"
#include "CFG.h"
#include "GDCore.h"
#include "TouchManager.h"

// Define touch area IDs
const std::string MainMenu::TOUCH_1PLAYER = "menu_1player";
const std::string MainMenu::TOUCH_OPTIONS = "menu_options";
const std::string MainMenu::TOUCH_ABOUT = "menu_about";

/* ******************************************** */

MainMenu::MainMenu(void) {
    this->lMO.push_back(new MenuOption("1 PLAYER GAME", 178, 276));
    this->lMO.push_back(new MenuOption("OPTIONS", 222, 308));
    this->lMO.push_back(new MenuOption("ABOUT", 237, 340));

    this->numOfMenuOptions = lMO.size();

    this->selectWorld = false;

    rSelectWorld.x = 122;
    rSelectWorld.y = 280;
    rSelectWorld.w = 306;
    rSelectWorld.h = 72;

    this->activeWorldID = this->activeSecondWorldID = 0;

    // Set up touch areas for menu options
    setupMenuTouchAreas();
}

MainMenu::~MainMenu(void) {
    // Remove menu touch areas when main menu is destroyed
    TouchManager::getInstance()->removeTouchArea(TOUCH_1PLAYER);
    TouchManager::getInstance()->removeTouchArea(TOUCH_OPTIONS);
    TouchManager::getInstance()->removeTouchArea(TOUCH_ABOUT);
}

/* ******************************************** */

void MainMenu::setupMenuTouchAreas() {
    // Only create touch areas if the manager exists
    if (!TouchManager::getInstance()) {
        return;
    }

    // Calculate touch area dimensions based on menu options
    // Make touch areas a bit larger than the text for easier touching
    const int TOUCH_PADDING = 10;  // Padding around text for touch area
    const int TOUCH_HEIGHT = 30;   // Fixed height for touch areas

    // Create touch area for "1 PLAYER GAME" option
    MenuOption* option1 = lMO[0];
    int width1 = CCFG::getText()->getTextWidth("1 PLAYER GAME", 16) + TOUCH_PADDING * 2;
    SDL_Rect bounds1 = {
            option1->getXPos() - TOUCH_PADDING,
            option1->getYPos() - TOUCH_PADDING,
            width1,
            TOUCH_HEIGHT
    };

    // Create touch area for "OPTIONS" option
    MenuOption* option2 = lMO[1];
    int width2 = CCFG::getText()->getTextWidth("OPTIONS", 16) + TOUCH_PADDING * 2;
    SDL_Rect bounds2 = {
            option2->getXPos() - TOUCH_PADDING,
            option2->getYPos() - TOUCH_PADDING,
            width2,
            TOUCH_HEIGHT
    };

    // Create touch area for "ABOUT" option
    MenuOption* option3 = lMO[2];
    int width3 = CCFG::getText()->getTextWidth("ABOUT", 16) + TOUCH_PADDING * 2;
    SDL_Rect bounds3 = {
            option3->getXPos() - TOUCH_PADDING,
            option3->getYPos() - TOUCH_PADDING,
            width3,
            TOUCH_HEIGHT
    };

    // Create touch areas with callbacks to select the appropriate option
    TouchManager::getInstance()->addTouchArea(TOUCH_1PLAYER, bounds1,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      selectMenuOption(0);
                                                  }
                                              });

    TouchManager::getInstance()->addTouchArea(TOUCH_OPTIONS, bounds2,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      selectMenuOption(1);
                                                  }
                                              });

    TouchManager::getInstance()->addTouchArea(TOUCH_ABOUT, bounds3,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      selectMenuOption(2);
                                                  }
                                              });

    // Set semi-transparent colors for touch areas (for debugging)
    SDL_Color normalColor = {200, 200, 200, 40};  // Very transparent gray
    SDL_Color pressedColor = {255, 255, 255, 80}; // Slightly more visible when pressed
    SDL_Color borderColor = {255, 255, 255, 60};  // Semi-transparent border

    // Apply colors to all menu touch areas
    for (const auto& id : {TOUCH_1PLAYER, TOUCH_OPTIONS, TOUCH_ABOUT}) {
        if (TouchArea* area = TouchManager::getInstance()->getTouchArea(id)) {
            area->normalColor = normalColor;
            area->pressedColor = pressedColor;
            area->borderColor = borderColor;
        }
    }
}

void MainMenu::selectMenuOption(int optionIndex) {
    // Change active menu option
    activeMenuOption = optionIndex;

    // Execute the action as if enter was pressed
    enter();
}

void MainMenu::Update() {
    CCFG::getMusic()->StopMusic();
    Menu::Update();
}

void MainMenu::Draw(SDL_Renderer* rR) {
//	CCFG::getSMBLOGO()->Draw(rR, 80, 48); TODO: Crash bug due to null error
    Menu::Draw(rR);
    CCFG::getText()->Draw(rR, "WWW.LUKASZJAKOWSKI.PL", 4, CCFG::GAME_HEIGHT - 4 - 8, 8, 0, 0, 0);
    CCFG::getText()->Draw(rR, "WWW.LUKASZJAKOWSKI.PL", 5, CCFG::GAME_HEIGHT - 5 - 8, 8, 255, 255, 255);

    if(selectWorld) {
        SDL_SetRenderDrawBlendMode(rR, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(rR, 4, 4, 4, 235);
        SDL_RenderFillRect(rR, &rSelectWorld);
        SDL_SetRenderDrawColor(rR, 255, 255, 255, 255);
        rSelectWorld.x += 1;
        rSelectWorld.y += 1;
        rSelectWorld.h -= 2;
        rSelectWorld.w -= 2;
        SDL_RenderDrawRect(rR, &rSelectWorld);
        rSelectWorld.x -= 1;
        rSelectWorld.y -= 1;
        rSelectWorld.h += 2;
        rSelectWorld.w += 2;

        CCFG::getText()->Draw(rR, "SELECT WORLD", rSelectWorld.x + rSelectWorld.w/2 - CCFG::getText()->getTextWidth("SELECT WORLD")/2, rSelectWorld.y + 16, 16, 255, 255, 255);

        for(int i = 0, extraX = 0; i < 8; i++) {
            if(i == activeWorldID) {
                CCFG::getText()->Draw(rR, std::to_string(i + 1) + "-" + std::to_string(activeSecondWorldID + 1), rSelectWorld.x + 16*(i + 1) + 16*i + extraX, rSelectWorld.y + 16 + 24, 16, 255, 255, 255);

                extraX = 32;

                /*for(int j = 0; j < 4; j++) {
                    if(j == activeSecondWorldID) {
                        CCFG::getText()->Draw(rR, std::to_string(j + 1), rSelectWorld.x + 16*(i + 1) + 16*i, rSelectWorld.y + 40 + 24*j, 16, 255, 255, 255);
                    } else {
                        CCFG::getText()->Draw(rR, std::to_string(j + 1), rSelectWorld.x + 16*(i + 1) + 16*i, rSelectWorld.y + 40 + 24*j, 16, 90, 90, 90);
                    }
                }*/
            } else {
                CCFG::getText()->Draw(rR, std::to_string(i + 1), rSelectWorld.x + 16*(i + 1) + 16*i + extraX, rSelectWorld.y + 16 + 24, 16, 90, 90, 90);
            }
        }

        SDL_SetRenderDrawBlendMode(rR, SDL_BLENDMODE_NONE);
        GDCore::getMap()->setBackgroundColor(rR);
    }
}

/* ******************************************** */

void MainMenu::enter() {
    switch(activeMenuOption) {
        case 0:
            if(!selectWorld) {
                selectWorld = true;
            } else {
                CCFG::getMM()->getLoadingMenu()->updateTime();
                GDCore::getMap()->resetGameData();
                GDCore::getMap()->setCurrentLevelID(activeWorldID * 4 + activeSecondWorldID);
                CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);
                CCFG::getMM()->getLoadingMenu()->loadingType = true;
                GDCore::getMap()->setSpawnPointID(0);
                selectWorld = false;
            }
            break;
        case 1:
            CCFG::getMM()->getOptions()->setEscapeToMainMenu(true);
            CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->eOptions);
            CCFG::getMM()->getOptions()->updateVolumeRect();
            CCFG::getMM()->setViewID(CCFG::getMM()->eOptions);
            break;
        case 2:
            CCFG::getMM()->getAboutMenu()->updateTime();
            CCFG::getMM()->setViewID(CCFG::getMM()->eAbout);
            CCFG::getMusic()->PlayMusic(CCFG::getMusic()->mOVERWORLD);
            break;
    }
}

void MainMenu::escape() {
    selectWorld = false;
}

void MainMenu::updateActiveButton(int iDir) {
    switch(iDir) {
        case 0: case 2:
            if(!selectWorld) {
                Menu::updateActiveButton(iDir);
            } else {
                switch(iDir) {
                    case 0:
                        if(activeSecondWorldID < 1) {
                            activeSecondWorldID = 3;
                        } else {
                            --activeSecondWorldID;
                        }
                        break;
                    case 2:
                        if(activeSecondWorldID > 2) {
                            activeSecondWorldID = 0;
                        } else {
                            ++activeSecondWorldID;
                        }
                        break;
                }
            }
            break;
        case 1:
            if(selectWorld) {
                if(activeWorldID < 7) {
                    ++activeWorldID;
                } else {
                    activeWorldID = 0;
                }
            }
            break;
        case 3:
            if(selectWorld) {
                if(activeWorldID > 0) {
                    --activeWorldID;
                } else {
                    activeWorldID = 7;
                }
            }
            break;
    }
}