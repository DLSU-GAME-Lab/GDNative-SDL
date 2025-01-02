//
// Created by delgallegon on 02/01/2025.
//
#include "GDCore.h"

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

//    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

//    window = SDL_CreateWindow("uMario - DLSU GAME LAB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CCFG::GAME_WIDTH, CCFG::GAME_HEIGHT, SDL_WINDOW_SHOWN);
    window = SDL_CreateWindow("uMario-Mobile - DLSU GAME LAB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 448, SDL_WINDOW_SHOWN);

    if(window == NULL) {
        quitGame = true;
    }

    rR = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // ----- ICO
//    std::string fileName = "files/images/ico.bmp";
//    SDL_Surface* loadedSurface = SDL_LoadBMP(fileName.c_str());
//    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));
//
//    SDL_SetWindowIcon(window, loadedSurface);

//    SDL_FreeSurface(loadedSurface);

    mainEvent = new SDL_Event();
    // ----- ICO

//    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
//
//    oMap = new Map(rR);
//    CCFG::getMM()->setActiveOption(rR);
//    CCFG::getSMBLOGO()->setIMG("super_mario_bros", rR);
//
//    CCFG::getMusic()->PlayMusic();

    this->keyMenuPressed = this->movePressed = this->keyS = this->keyW = this->keyA = this->keyD = this->keyShift = false;
    this->keyAPressed = this->keyDPressed = this->firstDir = false;
    this->mouseX = this->mouseY = 0;

//    CCFG::keyIDA = SDLK_a;
//    CCFG::keyIDS = SDLK_s;
//    CCFG::keyIDD = SDLK_d;
//    CCFG::keyIDSpace = SDLK_SPACE;
//    CCFG::keyIDShift = SDLK_LSHIFT;
}

GDCore::~GDCore(void) {
//    delete oMap;
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
//
////        CCFG::getMM()->setBackgroundColor(rR);
        SDL_RenderFillRect(rR, NULL);
//
//        Input();
//        MouseInput();
//        Update();
//        Draw();

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

void GDCore::Update() {

}

void GDCore::Draw() {

}

void GDCore::Input() {

}

void GDCore::MouseInput() {

}

void GDCore::InputPlayer() {

}

void GDCore::InputMenu() {

}

void GDCore::resetMove() {

}

void GDCore::resetKeys() {

}
