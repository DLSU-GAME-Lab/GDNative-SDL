#include "GemInputToken.h"
#include "SpriteRenderer.h"
#include "GemController.h"
#include "ButtonInput.h"

GemInputToken::GemInputToken(std::string strName, Vector2D position, Vector2D scale):AGameObject(strName)
{
	this->bDropped = false;
	this->fVecTranslate = position;
	this->fVecScale = scale;
}

void GemInputToken::initialize()
{
	SpriteRenderer* pRenderer = new SpriteRenderer(this->strName);
	ButtonInput* pInput = new ButtonInput(pRenderer);
	GemController* pController = new GemController(pInput);

	this->attachComponent(pRenderer);
	this->attachComponent(pInput);
	this->attachComponent(pController);
}

bool GemInputToken::getDropped() const
{
	return this->bDropped;
}
void GemInputToken::setDropped(bool bDropped)
{
	this->bDropped = bDropped;
}