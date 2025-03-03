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

void Button::OnGameStart() {
    this->bVisible = true;
    this->bPressedDown = false;
}

void Button::Draw(SDL_Renderer* rR, int iOffsetX, int iOffsetY) {
    this->pSprite->getTexture()->Draw(rR, iOffsetX, iOffsetY);
}

void Button::ToggleVisibility() {
    this->bVisible = !this->bVisible;
}