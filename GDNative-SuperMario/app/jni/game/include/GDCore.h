// GDCore.h
#ifndef GDCORE_H
#define GDCORE_H

#include "header.h"
#include "Map.h"
#include "TouchManager.h"

class GDCore {
private:
    SDL_Window* window;
    SDL_Renderer* rR;
    SDL_Event* mainEvent;

    unsigned int ticks;
    unsigned int iFPS, iNumOfFPS;
    long lFPSTime;

    int MIN_FRAME_TIME = 16;
    long frameTime;

    bool firstDir;

public:
    static Map* oMap;
    static bool mouseLeftPressed;
    static bool mouseRightPressed;
    static int mouseX, mouseY;
    static bool quitGame;
    static bool movePressed;
    static bool keyMenuPressed;
    static bool keyS, keyW;
    static bool keyA, keyD;
    static bool keyShift;
    static bool keyAPressed, keyDPressed;

    GDCore(void);
    ~GDCore(void);

    void mainLoop();
    void Input();
    void InputPlayer();
    void InputMenu();
    void MouseInput();

    void Update();
    void Draw();

    void resetMove();
    static void resetKeys();

    /* ----- get & set ----- */
    static Map* getMap();
};

#endif