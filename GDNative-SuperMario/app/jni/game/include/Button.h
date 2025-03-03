//
// Created by Kev on 03/03/2025.
//

#ifndef GDNATIVE_SUPERMARIO_BUTTON_H
#define GDNATIVE_SUPERMARIO_BUTTON_H

#include "IMG.h"
#include "Sprite.h"

class Button {
    private:
        bool bPressedDown;
        bool bVisible;
        Sprite* pSprite;

        enum eButtonType{
            eDPAD_UP,
            eDPAD_DOWN,
            eDPAD_LEFT,
            eDPAD_RIGHT,
            eA,
            eB,
            eNone
        };

        eButtonType eType;


    public:
        Button();
        Button(eButtonType eType);
        void OnGameStart();
};

#endif //GDNATIVE_SUPERMARIO_BUTTON_H
