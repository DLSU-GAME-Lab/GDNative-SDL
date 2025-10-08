#include "Gem.h"
#include "GemSwapper.h"
#include "TweenAnimator.h"

Gem::Gem(std::string strName, GemType EType) : AGameObject(strName)
{
	this->EType = EType;
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

	TweenAnimator* pTween = new TweenAnimator();
	this->attachComponent(pTween);
}

GemType Gem::getType() const
{
	return this->EType;
}
