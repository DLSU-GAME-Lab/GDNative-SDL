//
// Created by delgallegon on 02/01/2025.
//

#ifndef GDNATIVE_SUPERMARIO_GDCORE_H
#define GDNATIVE_SUPERMARIO_GDCORE_H

#include <map>
#include "Map.h"

//----- ImGui -----
#include "../ImGui/include/imgui.h"
#include "../ImGui/include/imgui_impl_sdl2.h"
#include "../ImGui/include/imgui_impl_sdlrenderer2.h"

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

    // ----- TOUCH
    int rendererOutputWindowWidth, rendererOutputWindowHeight;
    int logicalWidth, logicalHeight;
    float offsetX, offsetY;
    float scaledX, scaledY;

    struct TouchPoint {
        SDL_FingerID fingerId;
        ImVec2 pos;
        bool active;
    };

    std::map<SDL_FingerID, TouchPoint> touchPoints;

    // ----- Methods

    void Input();
    void MouseInput();
    void InputPlayer();
    void InputMenu();

    void TouchInput();
    void InitializeTouchVariables();
    void InitializeImGui();
    void RenderGameUI();
    bool IsPointInRect(const ImVec2& point, const ImVec2& rectMin, const ImVec2& rectMax);
    bool IsTouchingButton(const ImVec2& buttonPos, const ImVec2& buttonSize);
    ImVec2 GetTouchCoordinates(float touchX, float touchY);

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

    // ----- New Methods


    /* ----- get & set ----- */
    static Map* getMap();
};

#endif //GDNATIVE_SUPERMARIO_GDCORE_H
