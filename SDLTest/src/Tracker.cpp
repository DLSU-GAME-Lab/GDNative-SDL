#include "Tracker.h"
#include "GameObjectManager.h"
Tracker::Tracker(std::string strName, std::string strImageName, int nTargetDestroy):AGameObject(strName)
{
    this->nTargetLeft = this->nTargetDestroy = nTargetDestroy;
    this->strImageName = strImageName;
    this->fVecTranslate = Vector2D(0, 0);
    this->fVecScale = Vector2D(.075, .075);
}

Tracker::~Tracker()
{
}
void Tracker::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strImageName, this->fVecTranslate.x, this->fVecTranslate.y);
    this->attachComponent((AComponent*)pSpriteRenderer);

    Text* pCounterText = new Text(this->strName + "Text", std::to_string(this->nTargetLeft), Vector2D(-25, 0), Vector2D(.5, .5), 0, false);
    pCounterText->setFont("Maragasa45");
    this->attachChild(pCounterText);
}

std::string Tracker::getName()
{
    return this->strName;
}

int Tracker::getTargetLeft()
{
    return this->nTargetLeft;
}

void Tracker::setTargetLeft(int nTargetLeft)
{
    this->nTargetLeft = nTargetLeft;
}


