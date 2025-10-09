#include "Gem.h"
#include "GemSwapper.h"
#include "TweenAnimator.h"
#include "GemManager.h"

Gem::Gem(std::string strName, GemType EType) : AGameObject(strName)
{
	this->EType = EType;
	this->pTween = NULL;
	this->bActive = false;
}

Gem::~Gem()
{

}

void Gem::initialize()
{
	const std::string gemColors[] { "White", "Red", "Yellow", "Green", "Blue", "Purple" };
	SpriteRenderer* pRenderer = new SpriteRenderer(gemColors[(int)EType]);
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
		if (GemManager::getInstance()->checkMatches())
		{
			GemManager::getInstance()->destroyMatches();
		}
		else GemManager::getInstance()->moveGems();
		GemManager::getInstance()->clearSelection();
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

TweenAnimator* Gem::getTweenAnimator() const
{
	return this->pTween;
}
