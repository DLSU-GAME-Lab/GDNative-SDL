#include "MainMenu.h"
#include "CFG.h"
#include "GDCore.h"
#include "TouchManager.h"

// Define touch area IDs
const std::string MainMenu::TOUCH_1PLAYER = "mainmenu_1player";
const std::string MainMenu::TOUCH_OPTIONS = "mainmenu_options";
const std::string MainMenu::TOUCH_ABOUT = "mainmenu_about";
const std::string MainMenu::TOUCH_WORLD_PREFIX = "world_";
const std::string MainMenu::TOUCH_LEVEL_PREFIX = "level_";
const std::string MainMenu::TOUCH_WORLD_START = "world_start";
const std::string MainMenu::TOUCH_WORLD_BACK = "world_back";

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
    clearWorldSelectionTouchAreas();
}

/* ******************************************** */

void MainMenu::setupMenuTouchAreas() {
    // Create touch areas with appropriate dimensions
    const int TOUCH_WIDTH = 220;   // Wide enough for the text
    const int TOUCH_HEIGHT = 40;   // Tall enough to touch easily

    // Create touch area for "1 PLAYER GAME" option
    MenuOption* option1 = lMO[0];
    SDL_Rect bounds1 = {
            option1->getXPos(),
            option1->getYPos() - 15,
            TOUCH_WIDTH,
            TOUCH_HEIGHT
    };

    // Create touch area for "OPTIONS" option
    MenuOption* option2 = lMO[1];
    SDL_Rect bounds2 = {
            option2->getXPos(),
            option2->getYPos() - 15,
            TOUCH_WIDTH,
            TOUCH_HEIGHT
    };

    // Create touch area for "ABOUT" option
    MenuOption* option3 = lMO[2];
    SDL_Rect bounds3 = {
            option3->getXPos(),
            option3->getYPos() - 15,
            TOUCH_WIDTH,
            TOUCH_HEIGHT
    };

    // Create touch areas with callbacks
    TouchManager::getInstance()->addTouchArea(TOUCH_1PLAYER, bounds1,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      activeMenuOption = 0;
                                                      enter();
                                                  }
                                              });

    TouchManager::getInstance()->addTouchArea(TOUCH_OPTIONS, bounds2,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      activeMenuOption = 1;
                                                      enter();
                                                  }
                                              });

    TouchManager::getInstance()->addTouchArea(TOUCH_ABOUT, bounds3,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      activeMenuOption = 2;
                                                      enter();
                                                  }
                                              });

    // Set semi-transparent colors for the touch areas
    SDL_Color normalColor = {200, 200, 200, 60};   // Light gray, semi-transparent
    SDL_Color pressedColor = {255, 255, 255, 100}; // White, more visible when pressed
    SDL_Color borderColor = {255, 255, 255, 80};   // White border, semi-transparent

    // Apply colors to all menu touch areas
    for (const auto& id : {TOUCH_1PLAYER, TOUCH_OPTIONS, TOUCH_ABOUT}) {
        if (TouchArea* area = TouchManager::getInstance()->getTouchArea(id)) {
            area->normalColor = normalColor;
            area->pressedColor = pressedColor;
            area->borderColor = borderColor;
        }
    }
}

void MainMenu::setupWorldSelectionTouchAreas() {
    // Clear any existing world selection touch areas
    clearWorldSelectionTouchAreas();

    // Create touch areas for world selection (1-8)
    for (int i = 0; i < 8; i++) {
        // Calculate position based on the drawing logic
        int extraX = (i == activeWorldID) ? 32 : 0;
        int x = rSelectWorld.x + 16*(i + 1) + 16*i + extraX;
        int y = rSelectWorld.y + 16 + 24;

        // Create touch area
        std::string touchId = TOUCH_WORLD_PREFIX + std::to_string(i);
        SDL_Rect bounds = {x - 10, y - 10, 30, 30};  // Make it a bit larger for easier touching

        TouchManager::getInstance()->addTouchArea(touchId, bounds,
                                                  [this, i](bool pressed) {
                                                      if (pressed) {
                                                          // Set the active world ID
                                                          activeWorldID = i;
                                                      }
                                                  });

        // Set colors
        if (TouchArea* area = TouchManager::getInstance()->getTouchArea(touchId)) {
            area->normalColor = {200, 200, 200, 60};
            area->pressedColor = {255, 255, 255, 100};
            area->borderColor = {255, 255, 255, 80};
        }
    }

    // Add touch areas for level selection (1-4)
    for (int j = 0; j < 4; j++) {
        std::string touchId = TOUCH_LEVEL_PREFIX + std::to_string(j);
        // Position based on where these would be shown in the UI
        SDL_Rect bounds = {
                rSelectWorld.x + 150,
                rSelectWorld.y + 40 + 24*j,
                30, 30
        };

        TouchManager::getInstance()->addTouchArea(touchId, bounds,
                                                  [this, j](bool pressed) {
                                                      if (pressed) {
                                                          // Set the active secondary world ID
                                                          activeSecondWorldID = j;
                                                      }
                                                  });

        // Set colors
        if (TouchArea* area = TouchManager::getInstance()->getTouchArea(touchId)) {
            area->normalColor = {200, 200, 200, 60};
            area->pressedColor = {255, 255, 255, 100};
            area->borderColor = {255, 255, 255, 80};
        }
    }

    // Add a "Start Game" touch area
    SDL_Rect startBounds = {
            rSelectWorld.x + rSelectWorld.w/2 - 60,
            rSelectWorld.y + rSelectWorld.h - 30,
            120, 25
    };

    TouchManager::getInstance()->addTouchArea(TOUCH_WORLD_START, startBounds,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      // Do the same thing as pressing Enter on the world selection screen
                                                      CCFG::getMM()->getLoadingMenu()->updateTime();
                                                      GDCore::getMap()->resetGameData();
                                                      GDCore::getMap()->setCurrentLevelID(activeWorldID * 4 + activeSecondWorldID);
                                                      CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);
                                                      CCFG::getMM()->getLoadingMenu()->loadingType = true;
                                                      GDCore::getMap()->setSpawnPointID(0);
                                                      selectWorld = false;

                                                      // Clear the touch areas when leaving this screen
                                                      clearWorldSelectionTouchAreas();
                                                  }
                                              });

    // Add a "Back" touch area
    SDL_Rect backBounds = {
            rSelectWorld.x + 10,
            rSelectWorld.y + rSelectWorld.h - 30,
            50, 25
    };

    TouchManager::getInstance()->addTouchArea(TOUCH_WORLD_BACK, backBounds,
                                              [this](bool pressed) {
                                                  if (pressed) {
                                                      // Go back to main menu
                                                      selectWorld = false;

                                                      // Clear the touch areas when leaving this screen
                                                      clearWorldSelectionTouchAreas();
                                                  }
                                              });
}

void MainMenu::clearWorldSelectionTouchAreas() {
    // Remove all world selection touch areas
    for (int i = 0; i < 8; i++) {
        TouchManager::getInstance()->removeTouchArea(TOUCH_WORLD_PREFIX + std::to_string(i));
    }

    for (int j = 0; j < 4; j++) {
        TouchManager::getInstance()->removeTouchArea(TOUCH_LEVEL_PREFIX + std::to_string(j));
    }

    TouchManager::getInstance()->removeTouchArea(TOUCH_WORLD_START);
    TouchManager::getInstance()->removeTouchArea(TOUCH_WORLD_BACK);
}

void MainMenu::Update() {
    CCFG::getMusic()->StopMusic();

    if (!touchAreasInitialized) {
        setupMenuTouchAreas();
        touchAreasInitialized = true;
    }

    Menu::Update();
}

void MainMenu::Draw(SDL_Renderer* rR) {
    // Call the parent Draw method
    Menu::Draw(rR);

    // Render the copyright text
    CCFG::getText()->Draw(rR, "WWW.LUKASZJAKOWSKI.PL", 4, CCFG::GAME_HEIGHT - 4 - 8, 8, 0, 0, 0);
    CCFG::getText()->Draw(rR, "WWW.LUKASZJAKOWSKI.PL", 5, CCFG::GAME_HEIGHT - 5 - 8, 8, 255, 255, 255);

    // Handle world selection screen drawing
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
            } else {
                CCFG::getText()->Draw(rR, std::to_string(i + 1), rSelectWorld.x + 16*(i + 1) + 16*i + extraX, rSelectWorld.y + 16 + 24, 16, 90, 90, 90);
            }
        }

        SDL_SetRenderDrawBlendMode(rR, SDL_BLENDMODE_NONE);
        GDCore::getMap()->setBackgroundColor(rR);
    }

    // CRITICAL FIX: Explicitly draw touch areas
    SDL_SetRenderDrawBlendMode(rR, SDL_BLENDMODE_BLEND);
    TouchManager::getInstance()->drawTouchAreas(rR);
}

/* ******************************************** */

void MainMenu::enter() {
    switch(activeMenuOption) {
        case 0:
            if(!selectWorld) {
                selectWorld = true;
                // Create touch areas for world selection when entering this screen
                setupWorldSelectionTouchAreas();
            } else {
                CCFG::getMM()->getLoadingMenu()->updateTime();
                GDCore::getMap()->resetGameData();
                GDCore::getMap()->setCurrentLevelID(activeWorldID * 4 + activeSecondWorldID);
                CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);
                CCFG::getMM()->getLoadingMenu()->loadingType = true;
                GDCore::getMap()->setSpawnPointID(0);
                selectWorld = false;
                // Clear world selection touch areas
                clearWorldSelectionTouchAreas();
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
    // Clear world selection touch areas when escaping
    clearWorldSelectionTouchAreas();
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

void MainMenu::clearMenuTouchAreas() {
    TouchManager::getInstance()->removeTouchArea(TOUCH_1PLAYER);
    TouchManager::getInstance()->removeTouchArea(TOUCH_OPTIONS);
    TouchManager::getInstance()->removeTouchArea(TOUCH_ABOUT);
    touchAreasInitialized = false; // Reset the flag
}