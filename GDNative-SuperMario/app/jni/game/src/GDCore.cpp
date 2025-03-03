//
// Created by delgallegon on 02/01/2025.
//
#include "header.h"
#include "GDCore.h"
#include "IMG.h"
#include "CFG.h"
#include "Text.h"
#include "SDL_mixer.h"

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

// Initialize touch controls
TouchControl GDCore::dpadUp = {{0, 0, 0, 0}, "dpad_up", false};
TouchControl GDCore::dpadDown = {{0, 0, 0, 0}, "dpad_down", false};
TouchControl GDCore::dpadLeft = {{0, 0, 0, 0}, "dpad_left", false};
TouchControl GDCore::dpadRight = {{0, 0, 0, 0}, "dpad_right", false};
TouchControl GDCore::buttonA = {{0, 0, 0, 0}, "button_a", false};
TouchControl GDCore::buttonB = {{0, 0, 0, 0}, "button_b", false};
TouchControl GDCore::pauseButton = {{0, 0, 0, 0}, "pause", false};

GDCore::GDCore(void) {
    this->quitGame = false;
    this->iFPS = 0;
    this->iNumOfFPS = 0;
    this->lFPSTime = 0;

//    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

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
//    SDL_RenderSetScale(rR, 2.4f, 2.4f);


//     ----- ICO
    std::string fileName = "files/images/ico.bmp";
    SDL_Surface* loadedSurface = SDL_LoadBMP(fileName.c_str());
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));

    SDL_SetWindowIcon(window, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    mainEvent = new SDL_Event();
    // ----- ICO

//    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
//
    oMap = new Map(rR);
    CCFG::getMM()->setActiveOption(rR);
    CCFG::getSMBLOGO()->setIMG("super_mario_bros", rR);
//
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

    // Initialize touch controls
    initTouchControls();
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

        //the check to see if touch is within bounds of the buttons
        //where x as lower bound and (x + width) as upper bound, same for y and height
        /*
        if(this->CheckIfWithinBounds(screenX, screenY, dpad_left)) {
            //call func to move left
            CCFG::getMM()->keyPressed(3);
        }
        else if(this->CheckIfWithinBounds(screenX, screenY, dpad_right)) {
            //call func to move right
            CCFG::getMM()->keyPressed(1);
        }
        else if(this->CheckIfWithinBounds(screenX, screenY, dpad_up)) {
            //call func to move up
            CCFG::getMM()->keyPressed(4);
        }
        else if(this->CheckIfWithinBounds(screenX, screenY, dpad_down)) {
            //call func to move down
            CCFG::getMM()->keyPressed(2);
        }
        else if(this->CheckIfWithinBounds(screenX, screenY, aButton)) {
            CCFG::getMM()->enter();
            //call enter
        }
        else if(this->CheckIfWithinBounds(screenX, screenY, bButton)) {
            CCFG::getMM()->escape();
            //call escape
        }
        */
    }

    if(mainEvent->type == SDL_FINGERUP) {
        //when finger is lifted to stop movement, funcs, etc
    }
}

bool GDCore::CheckIfWithinBounds(int ScreenX, int ScreenY, Button button) {
    if(screenX >= button.GetXPos() && screenX <= (button.GetXPos() + button.GetWidth()) && screenY >= button.GetYPos() && screenY <= (button.GetYPos() + button.GetHeight())) {
        return true;
    }
    return false;
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
                if(!oMap->getUnderWater() && !oMap->getPlayer()->getInLevelAnimation()) oMap->getPlayer()->setSquat(true);
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
                    // Handle as touch for mobile controls
                    handleTouchEvents(mouseX, mouseY, true);
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
                handleTouchEvents(mouseX, mouseY, true);
            }
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            switch (mainEvent->button.button) {
                case SDL_BUTTON_LEFT:
                    mouseLeftPressed = false;
                    // Handle touch release
                    handleTouchEvents(mouseX, mouseY, false);
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
            handleTouchEvents(touchX, touchY, true);
            break;
        }
        case SDL_FINGERMOTION: {
            int touchX = (int)(mainEvent->tfinger.x * CCFG::GAME_WIDTH);
            int touchY = (int)(mainEvent->tfinger.y * CCFG::GAME_HEIGHT);
            mouseX = touchX;
            mouseY = touchY;
            handleTouchEvents(touchX, touchY, true);
            break;
        }
        case SDL_FINGERUP: {
            handleTouchEvents(mouseX, mouseY, false);
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

    // Draw touch controls after everything else
    drawTouchControls(rR);
}

/* ******************************************** */

void GDCore::resetMove() {
    this->keyAPressed = this->keyDPressed = false;
}

Map* GDCore::getMap() {
    return oMap;
}

// Initialize touch controls
void GDCore::initTouchControls()
{
    // D-pad
    int dpadSize = 50;
    int dpadSpacing = 125;
    int dpadY = CCFG::GAME_HEIGHT - 140;

    dpadLeft.bounds = {20, dpadY, dpadSize, dpadSize};
    dpadRight.bounds = {20 + dpadSpacing, dpadY, dpadSize, dpadSize};
    dpadUp.bounds = {20 + dpadSpacing/2, dpadY - dpadSize, dpadSize, dpadSize};
    dpadDown.bounds = {20 + dpadSpacing/2, dpadY + dpadSize, dpadSize, dpadSize};

    // A and B Buttons
    int buttonSize = 35;
    int buttonY = CCFG::GAME_HEIGHT - 140;

    buttonA.bounds = {CCFG::GAME_WIDTH - 100, buttonY, buttonSize, buttonSize};
    buttonB.bounds = {CCFG::GAME_WIDTH - 190, buttonY, buttonSize, buttonSize};

    // Pause button
    pauseButton.bounds = {CCFG::GAME_WIDTH - 50, 20, 40, 40};
}

// Draw touch controls
void GDCore::drawTouchControls(SDL_Renderer* renderer)
{
    // Only draw controls during gameplay
    if(CCFG::getMM()->getViewID() != 2 && CCFG::getMM()->getViewID() != 7)
    {
        return;
    }

    // Left button
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, dpadLeft.pressed ? 180 : 120);
    SDL_RenderFillRect(renderer, &dpadLeft.bounds);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderDrawRect(renderer, &dpadLeft.bounds);

    // Right button
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, dpadRight.pressed ? 180 : 120);
    SDL_RenderFillRect(renderer, &dpadRight.bounds);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderDrawRect(renderer, &dpadRight.bounds);

    // Up button
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, dpadUp.pressed ? 180 : 120);
    SDL_RenderFillRect(renderer, &dpadUp.bounds);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderDrawRect(renderer, &dpadUp.bounds);

    // Down button
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, dpadDown.pressed ? 180 : 120);
    SDL_RenderFillRect(renderer, &dpadDown.bounds);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderDrawRect(renderer, &dpadDown.bounds);

    // A button
    SDL_SetRenderDrawColor(renderer, 255, 50, 50, buttonA.pressed ? 180 : 120);
    SDL_RenderFillRect(renderer, &buttonA.bounds);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderDrawRect(renderer, &buttonA.bounds);

    // B button
    SDL_SetRenderDrawColor(renderer, 50, 50, 255, buttonB.pressed ? 180 : 120);
    SDL_RenderFillRect(renderer, &buttonB.bounds);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderDrawRect(renderer, &buttonB.bounds);
}

//Touch inputs
void GDCore::handleTouchEvents(int touchX, int touchY, bool isTouching)
{
    if (CCFG::getMM()->getViewID() != 2 && CCFG::getMM()->getViewID() != 7)
    {
        // Reset all touch controls when not in gameplay
        dpadUp.pressed = dpadDown.pressed = dpadLeft.pressed = dpadRight.pressed =
        buttonA.pressed = buttonB.pressed = pauseButton.pressed = false;
        return;
    }

    // Check each control
    if (isTouching) {
        // D-pad Up
        if (touchX >= dpadUp.bounds.x && touchX <= dpadUp.bounds.x + dpadUp.bounds.w &&
            touchY >= dpadUp.bounds.y && touchY <= dpadUp.bounds.y + dpadUp.bounds.h)
        {
            dpadUp.pressed = true;
            if (!CCFG::keySpace)
            {
                oMap->getPlayer()->jump();
                CCFG::keySpace = true;
            }
        }
        else
        {
            dpadUp.pressed = false;
        }

        // D-pad Down
        if (touchX >= dpadDown.bounds.x && touchX <= dpadDown.bounds.x + dpadDown.bounds.w &&
            touchY >= dpadDown.bounds.y && touchY <= dpadDown.bounds.y + dpadDown.bounds.h)
        {
            dpadDown.pressed = true;
            if (!keyS)
            {
                keyS = true;
                oMap->getPlayer()->jump();
            }
        }
        else
        {
            if (keyS && dpadDown.pressed)
            {
                keyS = false;
            }
            dpadDown.pressed = false;
        }

        // D-pad Left
        if (touchX >= dpadLeft.bounds.x && touchX <= dpadLeft.bounds.x + dpadLeft.bounds.w &&
            touchY >= dpadLeft.bounds.y && touchY <= dpadLeft.bounds.y + dpadLeft.bounds.h)
        {
            dpadLeft.pressed = true;
            if (!keyDPressed)
            {
                firstDir = false;
                oMap->getPlayer()->jump();
            }
        }
        else
        {
            if (dpadLeft.pressed)
            {
                dpadLeft.pressed = false;
                oMap->getPlayer()->jump();
            }
        }

        // D-pad Right
        if (touchX >= dpadRight.bounds.x && touchX <= dpadRight.bounds.x + dpadRight.bounds.w &&
            touchY >= dpadRight.bounds.y && touchY <= dpadRight.bounds.y + dpadRight.bounds.h)
        {
            dpadRight.pressed = true;
            if (!keyAPressed)
            {
                firstDir = true;
                oMap->getPlayer()->jump();
            }

        }
        else
        {
            if (dpadRight.pressed)
            {
                dpadRight.pressed = false;
                oMap->getPlayer()->jump();
            }
        }

        // Button A
        if (touchX >= buttonA.bounds.x && touchX <= buttonA.bounds.x + buttonA.bounds.w &&
            touchY >= buttonA.bounds.y && touchY <= buttonA.bounds.y + buttonA.bounds.h)
        {
            buttonA.pressed = true;
            if (!CCFG::keySpace)
            {
                oMap->getPlayer()->jump();
                CCFG::keySpace = true;
            }
        } else {
            buttonA.pressed = false;
        }

        // B Button
        if (touchX >= buttonB.bounds.x && touchX <= buttonB.bounds.x + buttonB.bounds.w &&
            touchY >= buttonB.bounds.y && touchY <= buttonB.bounds.y + buttonB.bounds.h)
        {
            buttonB.pressed = true;
            if (!keyShift)
            {
                keyShift = true;
            }
        }
        else
        {
            if (buttonB.pressed)
            {
                if (keyShift)
                {
                    keyShift = false;
                }
                buttonB.pressed = false;
            }
        }
    }
    else
    {
        //Reset controls
        if (dpadUp.pressed) {
            dpadUp.pressed = false;
            CCFG::keySpace = false;
        }

        if (dpadDown.pressed)
        {
            keyS = false;
            dpadDown.pressed = false;
        }

        if (dpadLeft.pressed)
        {
            keyAPressed = false;
            dpadLeft.pressed = false;
        }

        if (dpadRight.pressed)
        {
            keyDPressed = false;
            dpadRight.pressed = false;
        }

        if (buttonA.pressed)
        {
            CCFG::keySpace = false;
            buttonA.pressed = false;
        }

        if (buttonB.pressed)
        {
            if (keyShift)
            {
                keyShift = false;
            }
            buttonB.pressed = false;
        }
    }
}