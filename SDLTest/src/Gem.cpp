#include "Gem.h"
#include "GemSwapper.h"
#include "TweenAnimator.h"
#include "GemManager.h"

Gem::Gem(std::string strName, GemType EType) : AGameObject(strName)
{
	this->EType = EType;
	this->pTween = NULL;
	this->bActive = false;
	this->r = 0;
	this->c = 0;
}

Gem::~Gem()
{

}

void Gem::initialize()
{
	const std::string textures[] { "White", "Red", "Yellow", "Green", "Blue", "Purple", "BombS", "Crate" };
	int typeIndex = static_cast<int>(this->EType);
	if (typeIndex > 7) typeIndex = 7;
	SpriteRenderer* pRenderer = new SpriteRenderer(textures[typeIndex]);
	this->attachComponent(pRenderer);

	ButtonInput* pInput = new ButtonInput(pRenderer);
	this->attachComponent(pInput);

	GemSwapper* pSwapper = new GemSwapper(pInput);
	this->attachComponent(pSwapper);

	this->pTween = new TweenAnimator();
	this->pTween->addListener(this);
	this->attachComponent(pTween);
}

void Gem::onAnimationFinished()
{

	if (this->bActive)
	{
		this->bActive = false;
		GemManager::getInstance()->updateBoard();
	}
	else 
	{
		GemManager::getInstance()->finishAnimation();
	}

}

void Gem::setActive(bool bActive)
{
	this->bActive = bActive;
}

GemType Gem::getType() const
{
	return this->EType;
}

void Gem::setType(GemType EType)
{
	this->EType = EType;
}

Uint8 Gem::getRow() const
{
	return this->r;
}

Uint8 Gem::getCol() const
{
	return this->c;
}

void Gem::setGridPosition(Uint8 r, Uint8 c)
{
	this->r = r;
	this->c = c;
}

TweenAnimator* Gem::getTweenAnimator() const
{
	return this->pTween;
}
