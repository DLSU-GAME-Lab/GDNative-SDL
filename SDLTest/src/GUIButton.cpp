#include "GUIButton.h"
#include "ButtonInput.h"
#include "SpriteRenderer.h"

GUIButton::GUIButton(std::string strName, std::string strSpriteName, bool bFlipX) : AGameObject(strName)
{
	this->strSpriteName = strSpriteName;
	this->bFlipX = bFlipX;
}

void GUIButton::initialize()
{
	//this->bIsScreenObject = true;
	SpriteRenderer* pRenderer = new SpriteRenderer(this->strSpriteName);
	ButtonInput* pInput = new ButtonInput(pRenderer);

	if (this->bFlipX)
	{
		pRenderer->setFlipX(true);
	}
	this->attachComponent(pRenderer);
	this->attachComponent(pInput);
}
