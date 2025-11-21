#include "Scroller.h"

Scroller::Scroller(std::string strName, Text* pScrollableText):AComponent(strName, ComponentType::SCRIPT)
{
	this->pScrollableText = pScrollableText;
	this->bDragging = false;
    this->bVert = false;
    this->bHori = false;
}

Scroller::~Scroller()
{
}

void Scroller::perform()
{
	ButtonInput* pInput = (ButtonInput*)pOwner->findComponentByName("ButtonInput");
	DialogueRenderer* pTextRenderer = (DialogueRenderer*)this->pScrollableText->findComponentByName("DialogueRenderer");
	
    if (pInput->getDragging())
    {
        if (!bDragging)
        {
            bDragging = true;
            this->fPrevMouseX = pInput->getMousePos().x;
            this->fPrevMouseY = pInput->getMousePos().y;
        }
        else
        {
            float fDeltaX = 0;
            float fDeltaY = 0;
            if(this->bHori)
            {
                fDeltaX = pInput->getMousePos().x - this->fPrevMouseX;
                this->fPrevMouseX = pInput->getMousePos().x;
            }
            if(this->bVert)
            {
                fDeltaY = pInput->getMousePos().y - this->fPrevMouseY;
                this->fPrevMouseY = pInput->getMousePos().y;
            }

            float fX = this->pScrollableText->getPos().x + fDeltaX;

            float fY = this->pScrollableText->getPos().y + fDeltaY;
        
            this->pScrollableText->setPos(Vector2D(fX, fY));

        }
    }
    else
    {
        bDragging = false;
    }

}

void Scroller::setHori(bool bHori)
{
    this->bHori = bHori;
}

void Scroller::setVert(bool bVert)
{
    this->bVert = bVert;
}
