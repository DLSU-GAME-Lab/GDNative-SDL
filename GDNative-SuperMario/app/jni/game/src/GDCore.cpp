//
// Created by delgallegon on 02/01/2025.
//
#include "header.h"
#include "GDCore.h"
#include "IMG.h"
#include "CFG.h"
#include "Text.h"
#include "SDL_mixer.h"
#include "TouchManager.h" // Include the TouchManager

Map* GDCore::oMap = new Map();
bool GDCore::mouseLeftPressed = false;
bool GDCore::mouseRightPressed = false;
int GDCore::mouseX = 0;
int GDCore::mouseY = 0;
bool GDCore::quitGame = false;

bool GDCore::movePressed = false;
bool GDCore::keyMenuPressed = false;
bool GDCore::keyS = false;
bool GDCore::keyW = false;
bool GDCore::keyA = false;
bool GDCore::keyD = false;
bool GDCore::keyShift = false;
bool GDCore::keyAPressed = false;
bool GDCore::keyDPressed = false;

GDCore::GDCore(void) {
    this->quitGame = false;
    this->iFPS = 0;
    this->iNumOfFPS = 0;
    this->lFPSTime = 0;

    // Get screen dimensions
    int screenWidth, screenHeight;
    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);
    screenWidth = displayMode.w;
    screenHeight = displayMode.h;

    // Calculate scale factors
    float scaleX = (float)screenWidth / CCFG::GAME_WIDTH;
    float scaleY = (float)screenHeight / CCFG::GAME_HEIGHT;
    float scale = std::min(scaleX, scaleY);

    // Calculate window dimensions
    int windowWidth = CCFG::GAME_WIDTH * scale;
    int windowHeight = CCFG::GAME_HEIGHT * scale;

    window = SDL_CreateWindow("uMario - DLSU GAME LAB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

    if(window == NULL) {
        quitGame = true;
    }

    rR = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(rR, CCFG::GAME_WIDTH, CCFG::GAME_HEIGHT);

    // ----- ICO
    std::string fileName = "files/images/ico.bmp";
    SDL_Surface* loadedSurface = SDL_LoadBMP(fileName.c_str());
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));

    SDL_SetWindowIcon(window, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    mainEvent = new SDL_Event();
    // ----- ICO

    oMap = new Map(rR);
    CCFG::getMM()->setActiveOption(rR);
    CCFG::getSMBLOGO()->setIMG("super_mario_bros", rR);

    CCFG::getMusic()->PlayMusic();

    this->keyMenuPressed = this->movePressed = this->keyS = this->keyW = this->keyA = this->keyD = this->keyShift = false;
    this->keyAPressed = this->keyDPressed = this->firstDir = false;
    this->mouseX = this->mouseY = 0;

    CCFG::keyIDA = SDLK_a;
    CCFG::keyIDS = SDLK_s;
    CCFG::keyIDD = SDLK_d;
    CCFG::keyIDSpace = SDLK_SPACE;
    CCFG::keyIDShift = SDLK_LSHIFT;

    CCFG::getMusic()->LoadAllMusic();

    // Initialize TouchManager
    TouchManager::getInstance()->initialize(CCFG::GAME_WIDTH, CCFG::GAME_HEIGHT);
    TouchManager::getInstance()->setAllTouchAreasOpacity(0.5f);
    TouchManager::getInstance()->setActiveScene("main_menu");
    // DPAD LEFT callback
    if (TouchArea* left = TouchManager::getInstance()->getTouchArea(TouchControlID::DPAD_LEFT)) {
        left->setCallback([this](bool pressed) {
            keyAPressed = pressed;
            if (pressed && !keyDPressed) {
                firstDir = false;
            }

            if (pressed) {
                if (!oMap->getPlayer()->getMove() && !keyDPressed &&
                    !oMap->getPlayer()->getChangeMoveDirection() &&
                    !oMap->getPlayer()->getSquat()) {
                    oMap->getPlayer()->startMove();
                    oMap->getPlayer()->setMoveDirection(false);
                } else if (!keyDPressed && oMap->getPlayer()->getMoveSpeed() > 0 &&
                           firstDir != oMap->getPlayer()->getMoveDirection()) {
                    oMap->getPlayer()->setChangeMoveDirection();
                }
            } else if (!keyDPressed && oMap->getPlayer()->getMove()) {
                oMap->getPlayer()->resetMove();
            }
        });
    }

    // DPAD RIGHT callback
    if (TouchArea* right = TouchManager::getInstance()->getTouchArea(TouchControlID::DPAD_RIGHT)) {
        right->setCallback([this](bool pressed) {
            keyDPressed = pressed;
            if (pressed && !keyAPressed) {
                firstDir = true;
            }

            if (pressed) {
                if (!oMap->getPlayer()->getMove() && !keyAPressed &&
                    !oMap->getPlayer()->getChangeMoveDirection() &&
                    !oMap->getPlayer()->getSquat()) {
                    oMap->getPlayer()->startMove();
                    oMap->getPlayer()->setMoveDirection(true);
                } else if (!keyAPressed && oMap->getPlayer()->getMoveSpeed() > 0 &&
                           firstDir != oMap->getPlayer()->getMoveDirection()) {
                    oMap->getPlayer()->setChangeMoveDirection();
                }
            } else if (!keyAPressed && oMap->getPlayer()->getMove()) {
                oMap->getPlayer()->resetMove();
            }
        });
    }

    // DPAD UP / JUMP callback
    if (TouchArea* up = TouchManager::getInstance()->getTouchArea(TouchControlID::DPAD_UP)) {
        up->setCallback([this](bool pressed) {
            if (pressed && !CCFG::keySpace) {
                oMap->getPlayer()->jump();
                CCFG::keySpace = true;
            } else if (!pressed) {
                CCFG::keySpace = false;
            }
        });
    }

    // DPAD DOWN / SQUAT callback
    if (TouchArea* down = TouchManager::getInstance()->getTouchArea(TouchControlID::DPAD_DOWN)) {
        down->setCallback([this](bool pressed) {
            keyS = pressed;
            if (pressed) {
                if (!oMap->getUnderWater() && !oMap->getPlayer()->getInLevelAnimation()) {
                    oMap->getPlayer()->setSquat(true);
                }
            } else {
                oMap->getPlayer()->setSquat(false);
            }
        });
    }

    // BUTTON A callback (jump/enter)
    if (TouchArea* buttonA = TouchManager::getInstance()->getTouchArea(TouchControlID::BUTTON_A)) {
        buttonA->setCallback([this](bool pressed) {
            if (pressed) {
                if (!CCFG::keySpace) {
                    oMap->getPlayer()->jump();
                    CCFG::keySpace = true;
                }
                if (!keyMenuPressed) {
                    CCFG::getMM()->enter();
                    keyMenuPressed = true;
                }
            } else {
                CCFG::keySpace = false;
                keyMenuPressed = false;
            }
        });
    }

    // BUTTON B callback (run)
    if (TouchArea* buttonB = TouchManager::getInstance()->getTouchArea(TouchControlID::BUTTON_B)) {
        buttonB->setCallback([this](bool pressed) {
            keyShift = pressed;
            if (pressed) {
                oMap->getPlayer()->startRun();
            } else {
                oMap->getPlayer()->resetRun();
            }
        });
    }

    // PAUSE button callback
    if (TouchArea* pause = TouchManager::getInstance()->getTouchArea(TouchControlID::PAUSE)) {
        pause->setCallback([this](bool pressed) {
            if (pressed && !keyMenuPressed && CCFG::getMM()->getViewID() == CCFG::getMM()->eGame) {
                CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->ePasue);
                CCFG::getMM()->setViewID(CCFG::getMM()->ePasue);
                CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPASUE);
                CCFG::getMusic()->PauseMusic();
                keyMenuPressed = true;
            } else if (!pressed) {
                keyMenuPressed = false;
            }
        });
    }
}

GDCore::~GDCore(void) {
    delete oMap;
    delete mainEvent;
    SDL_DestroyRenderer(rR);
    SDL_DestroyWindow(window);
}

/* ******************************************** */

void GDCore::mainLoop() {
    lFPSTime = SDL_GetTicks();

    while(!quitGame && mainEvent->type != SDL_QUIT) {
        frameTime = SDL_GetTicks();
        SDL_PollEvent(mainEvent);
        SDL_RenderClear(rR);

        CCFG::getMM()->setBackgroundColor(rR);
        SDL_RenderFillRect(rR, NULL);

        Input();
        MouseInput();
        Update();
        Draw();

        /*CCFG::getText()->Draw(rR, "FPS:" + std::to_string(iNumOfFPS), CCFG::GAME_WIDTH - CCFG::getText()->getTextWidth("FPS:" + std::to_string(iNumOfFPS), 8) - 8, 5, 8);

        if(SDL_GetTicks() - 1000 >= lFPSTime) {
            lFPSTime = SDL_GetTicks();
            iNumOfFPS = iFPS;
            iFPS = 0;
        }

        ++iFPS;*/

        SDL_RenderPresent(rR);

        if(SDL_GetTicks() - frameTime < MIN_FRAME_TIME) {
            SDL_Delay(MIN_FRAME_TIME - (SDL_GetTicks () - frameTime));
        }

        this->ticks = SDL_GetTicks64();
        if (this->ticks % 50 == 0) {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Total ticks so far: %d", this->ticks);
        }
    }
}

void GDCore::Input() {
    switch(CCFG::getMM()->getViewID()) {
        case 2: case 7:
            if(!oMap->getInEvent()) {
                InputPlayer();
            } else {
                resetMove();
            }
            break;
        default:
            InputMenu();
            break;
    }
}

void GDCore::InputMenu() {
    if(mainEvent->type == SDL_KEYDOWN) {
        CCFG::getMM()->setKey(mainEvent->key.keysym.sym);

        switch(mainEvent->key.keysym.sym) {
            case SDLK_s: case SDLK_DOWN:
                if(!keyMenuPressed) {
                    CCFG::getMM()->keyPressed(2);
                    keyMenuPressed = true;
                }
                break;
            case SDLK_w: case SDLK_UP:
                if(!keyMenuPressed) {
                    CCFG::getMM()->keyPressed(0);
                    keyMenuPressed = true;
                }
                break;
            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(!keyMenuPressed) {
                    CCFG::getMM()->enter();
                    keyMenuPressed = true;
                }
                break;
            case SDLK_ESCAPE:
                if(!keyMenuPressed) {
                    CCFG::getMM()->escape();
                    keyMenuPressed = true;
                }
                break;
            case SDLK_LEFT: case SDLK_d:
                if(!keyMenuPressed) {
                    CCFG::getMM()->keyPressed(3);
                    keyMenuPressed = true;
                }
                break;
            case SDLK_RIGHT: case SDLK_a:
                if(!keyMenuPressed) {
                    CCFG::getMM()->keyPressed(1);
                    keyMenuPressed = true;
                }
                break;
        }
    }

    if(mainEvent->type == SDL_KEYUP) {
        switch(mainEvent->key.keysym.sym) {
            case SDLK_s: case SDLK_DOWN: case SDLK_w: case SDLK_UP: case SDLK_KP_ENTER: case SDLK_RETURN: case SDLK_ESCAPE: case SDLK_a: case SDLK_RIGHT: case SDLK_LEFT: case SDLK_d:
                keyMenuPressed = false;
                break;
            default:
                break;
        }
    }

    if(mainEvent->type == SDL_FINGERDOWN) {
        //temporarily stores finger coords
        float touchX = mainEvent->tfinger.x;
        float touchY = mainEvent->tfinger.y;

        //convert to screen coords
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        int screenX = (int)(touchX * windowWidth);
        int screenY = (int)(touchY * windowHeight);

        // Handle touch events through TouchManager
        TouchManager::getInstance()->handleTouchEvent(mouseX, mouseY, true);
    }

    if(mainEvent->type == SDL_FINGERUP) {
        float touchX = mainEvent->tfinger.x;
        float touchY = mainEvent->tfinger.y;

        //convert to screen coords
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        int screenX = (int)(touchX * windowWidth);
        int screenY = (int)(touchY * windowHeight);

        // Handle touch release through TouchManager
        TouchManager::TouchManager::getInstance()->handleTouchEvent(screenX, screenY, false);
    }
}

void GDCore::InputPlayer() {
    if(mainEvent->type == SDL_WINDOWEVENT) {
        switch(mainEvent->window.event) {
            case SDL_WINDOWEVENT_FOCUS_LOST:
                CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->ePasue);
                CCFG::getMM()->setViewID(CCFG::getMM()->ePasue);
                CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPASUE);
                CCFG::getMusic()->PauseMusic();
                break;
        }
    }

    if(mainEvent->type == SDL_KEYUP) {
        if(mainEvent->key.keysym.sym == CCFG::keyIDD) {
            if(firstDir) {
                firstDir = false;
            }

            keyDPressed = false;
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDS) {
            oMap->getPlayer()->setSquat(false);
            keyS = false;
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDA) {
            if(!firstDir) {
                firstDir = true;
            }

            keyAPressed = false;
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDSpace) {
            CCFG::keySpace = false;
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDShift) {
            if(keyShift) {
                oMap->getPlayer()->resetRun();
                keyShift = false;
            }
        }
        switch(mainEvent->key.keysym.sym) {
            case SDLK_KP_ENTER: case SDLK_RETURN: case SDLK_ESCAPE:
                keyMenuPressed = false;
                break;
        }
    }

    if(mainEvent->type == SDL_KEYDOWN) {
        if(mainEvent->key.keysym.sym == CCFG::keyIDD) {
            keyDPressed = true;
            if(!keyAPressed) {
                firstDir = true;
            }
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDS) {
            if(!keyS) {
                keyS = true;
                if(!oMap->getUnderWater() && !oMap->getPlayer()->getInLevelAnimation())
                    oMap->getPlayer()->setSquat(true);
            }
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDA) {
            keyAPressed = true;
            if(!keyDPressed) {
                firstDir = false;
            }
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDSpace) {
            if(!CCFG::keySpace) {
                oMap->getPlayer()->jump();
                CCFG::keySpace = true;
            }
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDShift) {
            if(!keyShift) {
                oMap->getPlayer()->startRun();
                keyShift = true;
            }
        }

        switch(mainEvent->key.keysym.sym) {
            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(!keyMenuPressed) {
                    CCFG::getMM()->enter();
                    keyMenuPressed = true;
                }
            case SDLK_ESCAPE:
                if(!keyMenuPressed && CCFG::getMM()->getViewID() == CCFG::getMM()->eGame) {
                    CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->ePasue);
                    CCFG::getMM()->setViewID(CCFG::getMM()->ePasue);
                    CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPASUE);
                    CCFG::getMusic()->PauseMusic();
                    keyMenuPressed = true;
                }
                break;
        }
    }

    if(keyAPressed) {
        if(!oMap->getPlayer()->getMove() && firstDir == false && !oMap->getPlayer()->getChangeMoveDirection() && !oMap->getPlayer()->getSquat()) {
            oMap->getPlayer()->startMove();
            oMap->getPlayer()->setMoveDirection(false);
        } else if(!keyDPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
            oMap->getPlayer()->setChangeMoveDirection();
        }
    }

    if(keyDPressed) {
        if(!oMap->getPlayer()->getMove() && firstDir == true && !oMap->getPlayer()->getChangeMoveDirection() && !oMap->getPlayer()->getSquat()) {
            oMap->getPlayer()->startMove();
            oMap->getPlayer()->setMoveDirection(true);
        } else if(!keyAPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
            oMap->getPlayer()->setChangeMoveDirection();
        }
    }

    if(oMap->getPlayer()->getMove() && !keyAPressed && !keyDPressed) {
        oMap->getPlayer()->resetMove();
    }
}

void GDCore::MouseInput() {

    switch(mainEvent->type) {
        case SDL_MOUSEBUTTONDOWN: {
            switch (mainEvent->button.button) {
                case SDL_BUTTON_LEFT:
                    mouseLeftPressed = true;
                    // Handle as touch event
                    TouchManager::getInstance()->handleTouchEvent(mouseX, mouseY, true);
                    break;
                case SDL_BUTTON_RIGHT:
                    mouseRightPressed = true;
                    break;
            }
            break;
        }
        case SDL_MOUSEMOTION: {
            SDL_GetMouseState(&mouseX, &mouseY);

            // If left mouse button is held, treat as touch motion
            if(mouseLeftPressed) {
                TouchManager::getInstance()->handleTouchEvent(mouseX, mouseY, true);
            }
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            switch (mainEvent->button.button) {
                case SDL_BUTTON_LEFT:
                    mouseLeftPressed = false;
                    // Handle touch release
                    TouchManager::getInstance()->handleTouchEvent(mouseX, mouseY, false);
                    break;
                case SDL_BUTTON_RIGHT:
                    mouseRightPressed = false;
                    break;
            }
            break;
        }

            // Add touch event handling for mobile
        case SDL_FINGERDOWN: {
            int touchX = (int)(mainEvent->tfinger.x * CCFG::GAME_WIDTH);
            int touchY = (int)(mainEvent->tfinger.y * CCFG::GAME_HEIGHT);
            mouseX = touchX;
            mouseY = touchY;
            TouchManager::getInstance()->handleTouchEvent(touchX, touchY, true);
            break;
        }
        case SDL_FINGERMOTION: {
            int touchX = (int)(mainEvent->tfinger.x * CCFG::GAME_WIDTH);
            int touchY = (int)(mainEvent->tfinger.y * CCFG::GAME_HEIGHT);
            mouseX = touchX;
            mouseY = touchY;
            TouchManager::getInstance()->handleTouchEvent(touchX, touchY, true);
            break;
        }
        case SDL_FINGERUP: {
            TouchManager::getInstance()->handleTouchEvent(mouseX, mouseY, false);
            break;
        }

        case SDL_MOUSEWHEEL:
            if(mainEvent->wheel.timestamp > SDL_GetTicks() - 2) {
                //CCFG::getMM()->getLE()->mouseWheel(mainEvent->wheel.y);
            }
            break;
    }
}

void GDCore::resetKeys() {
    movePressed = keyMenuPressed = keyS = keyW = keyA = keyD = CCFG::keySpace = keyShift = keyAPressed = keyDPressed = false;
}

void GDCore::Update() {
    CCFG::getMM()->Update();
}

void GDCore::Draw() {
    CCFG::getMM()->Draw(rR);

    // Only draw touch controls during gameplay
    if (CCFG::getMM()->getViewID() == 2 || CCFG::getMM()->getViewID() == 7)
    {

        TouchManager::getInstance()->drawTouchAreas(rR);
    }
}

/* ******************************************** */

void GDCore::resetMove() {
    this->keyAPressed = this->keyDPressed = false;
}

Map* GDCore::getMap() {
    return oMap;
}