//
// Created by delgallegon on 02/01/2025.
//

#ifndef GDNATIVE_SUPERMARIO_GDCORE_H
#define GDNATIVE_SUPERMARIO_GDCORE_H

#include "Map.h"

class GDCore
{
private:
    SDL_Window* window;
    SDL_Renderer* rR;
    SDL_Event* mainEvent;

    uint64_t ticks;

    // ----- FPS -----

    long frameTime;
    static const int MIN_FRAME_TIME = 16;

    unsigned long lFPSTime;
    int iNumOfFPS, iFPS;

    // ----- FPS -----

    // ----- INPUT
    static bool movePressed, keyMenuPressed, keyS, keyW, keyA, keyD, keyShift;

    static bool keyAPressed, keyDPressed;
    // ----- true = RIGHT, false = LEFT
    bool firstDir;
    // ----- INPUT

    static Map* oMap;

    // ----- Methods

    void Input();
    void MouseInput();
    void InputPlayer();
    void InputMenu();

public:
    GDCore(void);
    ~GDCore(void);

    static bool quitGame;

    void mainLoop();

    void Update();
    void Draw();

    void resetMove();
    static void resetKeys();

    static bool mouseLeftPressed, mouseRightPressed;
    static int mouseX, mouseY;

    /* ----- get & set ----- */
    static Map* getMap();
};

#endif //GDNATIVE_SUPERMARIO_GDCORE_H
