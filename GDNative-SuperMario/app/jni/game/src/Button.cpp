//
// Created by Kev on 03/03/2025.
//
#include "Button.h"

Button::Button() {
    this->bVisible = false;
    this->bPressedDown = false;
    this->eType = eButtonType::eNone;
}

Button::Button(Button::eButtonType eType) {
    switch (eType) {
        case eA : {
            this->pSprite
            break;
        }
    }
}

void Button::OnGameStart() {
    this->bVisible = true;
    this->bPressedDown = false;
}