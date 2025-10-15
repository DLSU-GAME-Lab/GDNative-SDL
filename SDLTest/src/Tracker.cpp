#include "Tracker.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
Tracker::Tracker(std::string strName, std::string strImageName, GemType EGem, int nTargetDestroy):AGameObject(strName)
{
    this->nTargetLeft = this->nTargetDestroy = nTargetDestroy;
    this->strImageName = strImageName;
    this->fVecTranslate = Vector2D(0, 0);
    this->fVecScale = Vector2D(.075, .075);
    this->EGem = EGem;
}

Tracker::~Tracker()
{
}
void Tracker::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strImageName, this->fVecTranslate.x, this->fVecTranslate.y);
    this->attachComponent((AComponent*)pSpriteRenderer);
	std::cout << this->strName << std::endl;

    Text* pCounterText = new Text(this->strName, std::to_string(this->nTargetLeft), Vector2D(-25, 0), Vector2D(.5, .5), 0, false);
    pCounterText->setFont("Maragasa45");
    this->attachChild(pCounterText);
}

void Tracker::updateScore(int nSubtractValue)
{
    int nValueHolder = this->nTargetLeft;
    this->nTargetLeft = nValueHolder - nSubtractValue;
    if (this->nTargetLeft < 0)
    {
        this->nTargetLeft = 0;
    }
    Text* pText = (Text*)this->findChildByName(this->getName());
    pText->modifyText(std::to_string(this->nTargetLeft));
}

std::string Tracker::getName()
{
    return this->strName;
}

GemType Tracker::getGemType()
{
    return this->EGem;
}




