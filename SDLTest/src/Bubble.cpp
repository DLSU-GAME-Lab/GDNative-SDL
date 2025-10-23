#include "Bubble.h"
#include "SpriteRenderer.h"
#include "Settings.h"
#include "BubbleManager.h"

Bubble::Bubble(std::string strName) : AGameObject(strName)
{
	this->pAnimator = NULL;
	this->bIsScreenObject = true;
}

void Bubble::initialize()
{
	SpriteRenderer* pRenderer = new SpriteRenderer("Bubble");
	pRenderer->setColor({ 255, 255, 255, 200 });
	this->attachComponent(pRenderer);

	this->pAnimator = new TweenAnimator();
	this->pAnimator->addListener(this);
	this->attachComponent(pAnimator);
}

void Bubble::onAnimationFinished()
{
	BubbleManager::getInstance()->deactivateBubble(this);
}

void Bubble::randomizeTween()
{
	float randX = rand() % (int)gameWidth;
	this->pAnimator->setTweenPos(Tween2D::from(randX, gameHeight).to(randX, -200.0f).during(10000 / this->fVecScale.x));
	this->pAnimator->play();
}
