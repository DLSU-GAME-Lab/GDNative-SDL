//
// Created by Kev on 03/03/2025.
//
#include "Button.h"
#include "GDCore.h"

Button::Button(int iButtonID, int xPos, int yPos, float fWidth, float fHeight, Sprite* pSprite, eButtonType eType){
    this->iButtonID = iButtonID;
    this->bPressedDown = false;
    this->bVisible = true;
    this->xPos = xPos;
    this->yPos = yPos;
    this->fWidth = fWidth;
    this->fHeight = fHeight;
    this->pSprite = pSprite;
    this->eType = eType;
}

Button::~Button(void) {
    delete pSprite;
}

void Button::OnGameStart() {
    this->bVisible = true;
    this->bPressedDown = false;
}

void Button::Draw(SDL_Renderer* rR, int iOffsetX, int iOffsetY) {
    this->pSprite->getTexture()->Draw(rR, iOffsetX, iOffsetY);
    this->xPos = iOffsetX;
    this->yPos = iOffsetY;
}

void Button::SetPressed(bool pressed) {
    this->bPressedDown = pressed;
}

bool Button::IsPressed() const {
    return bPressedDown;
}

void Button::SetVisible(bool visible) {
    this->bVisible = visible;
}

void Button::ToggleVisibility() {
    this->bVisible = !this->bVisible;
}

int Button::GetXPos() {
    return this->xPos;
}

int Button::GetYPos() {
    return this->yPos;
}

int Button::GetWidth() {
    return (int)(this->fWidth);
}

int Button::GetHeight() {
    return (int)(this->fHeight);
}

Button::eButtonType Button::GetType() {
    return this->eType; // Changed from "etype" to "eType"
}