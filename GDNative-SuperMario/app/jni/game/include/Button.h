//
// Created by Kev on 03/03/2025.
//

#ifndef GDNATIVE_SUPERMARIO_BUTTON_H
#define GDNATIVE_SUPERMARIO_BUTTON_H

#include "IMG.h"
#include "Sprite.h"

class Button {
public:
    enum eButtonType {
        eDPAD_UP,
        eDPAD_DOWN,
        eDPAD_LEFT,
        eDPAD_RIGHT,
        eA,
        eB,
        eStart,
        eSelect,
        eNone
    };

private:
    int iButtonID;
    bool bPressedDown;
    bool bVisible;
    int xPos, yPos;
    float fWidth, fHeight;
    Sprite* pSprite;
    eButtonType eType; // Added member variable for button type

public:
    Button(int iButtonID, int xPos, int yPos, float fWidth, float fHeight, Sprite* pSprite, eButtonType eType);
    ~Button(void);
    void OnGameStart();

    void Draw(SDL_Renderer* rR, int iOffsetX, int iOffsetY);
    void SetPressed(bool pressed);
    bool IsPressed() const;
    void SetVisible(bool visible);
    void ToggleVisibility();
    eButtonType GetType();

    int GetXPos();
    int GetYPos();
    int GetWidth();
    int GetHeight();
};

#endif //GDNATIVE_SUPERMARIO_BUTTON_H