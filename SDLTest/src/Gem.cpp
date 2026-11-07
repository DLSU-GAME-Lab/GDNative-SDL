#include "Gem.h"
#include "GemSwapper.h"
#include "TweenAnimator.h"
#include "GemManager.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include <cstdlib>
// texture key of a gem type, crates show State_3..1 (CRATE_0..2) as they take damage
// and fall back to the plain "Crate" art while the state art isn't loaded
static std::string getTextureKey(GemType EType)
{
	const std::string textures[]{ "White", "Red", "Yellow", "Green", "Blue", "Purple", "BombS" };
	int typeIndex = static_cast<int>(EType);
	int crateIndex = static_cast<int>(GemType::CRATE_0);

	if (typeIndex >= crateIndex)
	{
		int nOffset = typeIndex - crateIndex;
		if (nOffset > 2) nOffset = 2;
		int nState = 3 - nOffset;
		std::string strState = "State_" + std::to_string(nState);
		return TextureManager::getInstance()->get(strState) ? strState : "Crate";
	}

	if (typeIndex > 6) typeIndex = 6;
	return textures[typeIndex];
}

Gem::Gem(std::string strName, GemType EType) : AGameObject(strName)
{
	this->EType = EType;
	this->pTween = NULL;
	this->pSprite = NULL;
	this->bActive = false;
	this->r = 0;
	this->c = 0;
}

Gem::~Gem()
{

}

void Gem::initialize()
{
	SpriteRenderer* pRenderer = new SpriteRenderer(getTextureKey(this->EType));
	this->pSprite = pRenderer;
	this->attachComponent(pRenderer);

	ButtonInput* pInput = new ButtonInput(pRenderer);
	this->attachComponent(pInput);

	GemSwapper* pSwapper = new GemSwapper(pInput);
	this->attachComponent(pSwapper);

	this->pTween = new TweenAnimator();
	this->pTween->addListener(this);
	this->attachComponent(pTween);

	// the sweeps are spread out at random
	const float fSpeed = 0.45f;
	const float fCooldown = 5.0f;
	float fOffset = (rand() % 1000) / 1000.0f * (1.0f / fSpeed + fCooldown);
	pRenderer->setShimmerTiming(fSpeed, fCooldown, fOffset);

	this->applyAppearance();
}

// texture and shimmer follow the gem type, gems shimmer only when their art has a
// normal map loaded as "<texture>_Normal" (and optionally a mask as "<texture>_Mask")
void Gem::applyAppearance()
{
	std::string strTexture = getTextureKey(this->EType);
	this->pSprite->setTextureKey(strTexture);

	this->pSprite->setShimmerFromTexture(strTexture);
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
	if (this->pSprite) this->applyAppearance();   // e.g. a damaged crate swaps its state art
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