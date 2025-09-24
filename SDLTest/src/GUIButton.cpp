#include "GUIButton.h"
#include "ButtonInput.h"
#include "SpriteRenderer.h"

GUIButton::GUIButton(std::string strName, std::string strSpriteName) : AGameObject(strName)
{
	this->strSpriteName = strSpriteName;
}

GUIButton::~GUIButton()
{

}

void GUIButton::initialize()
{
	SpriteRenderer* pRenderer = new SpriteRenderer(this->strSpriteName);
	ButtonInput* pInput = new ButtonInput(pRenderer);

	this->attachComponent(pRenderer);
	this->attachComponent(pInput);
}
