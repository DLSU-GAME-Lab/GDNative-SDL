#include "Scroller.h"

Scroller::Scroller(std::string strName, Text* pScrollableText):AComponent(strName, ComponentType::SCRIPT)
{
	this->pScrollableText = pScrollableText;
	this->bDragging = false;
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
            float fDeltaX = pInput->getMousePos().x - this->fPrevMouseX;
            float fDeltaY = pInput->getMousePos().y - this->fPrevMouseY;
            this->fPrevMouseX = pInput->getMousePos().x;
            this->fPrevMouseY = pInput->getMousePos().y;

            

            this->pScrollableText->setPos(Vector2D(this->pScrollableText->getPos().x + fDeltaX, this->pScrollableText->getPos().y + fDeltaY));

        }
    }
    else
    {
        bDragging = false;
    }

}
