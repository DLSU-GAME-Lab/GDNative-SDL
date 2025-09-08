#include "Prop.h"


Prop::Prop(const std::string& strName, const std::string& strImageName, float fX, float fY, float fW, float fH, float fRot, bool bFlipX):
	AGameObject(strName), strImageName(strImageName),bFlipX(bFlipX)
{
	this->fPosX = fX;
	this->fPosY = fY;
	this->fScaleX = fW;
	this->fScaleY = fH;
	this->fRot = fRot;
}

Prop::~Prop()
{
}

void Prop::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strImageName, this->fPosX, this->fPosY);
	pSpriteRenderer->setScale(fScaleX, fScaleY);
	if (bFlipX)
	{
		pSpriteRenderer->setFlipX(true);
	}
	SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
	this->attachComponent((AComponent*)pSpriteRenderer);
}

