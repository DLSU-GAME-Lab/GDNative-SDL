// GDCore.h
#ifndef GDCORE_H
#define GDCORE_H

#include "header.h"
#include "Map.h"

struct TouchControl {
    SDL_Rect bounds;
    const char* name;
    bool pressed;
};

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

    // Touch controls
    static TouchControl dpadUp;
    static TouchControl dpadDown;
    static TouchControl dpadLeft;
    static TouchControl dpadRight;
    static TouchControl buttonA;
    static TouchControl buttonB;
    static TouchControl pauseButton;

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

    void initTouchControls();
    void drawTouchControls(SDL_Renderer* renderer);
    void handleTouchEvents(int touchX, int touchY, bool isTouching);
};

#endif