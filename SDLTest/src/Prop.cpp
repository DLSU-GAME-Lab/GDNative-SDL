#include "Prop.h"


Prop::Prop(const std::string& strName, const std::string& strImageName, Vector2D fVecTranslate, Vector2D fVecScale, float fRot, bool bFlipX):
	AGameObject(strName), strImageName(strImageName),bFlipX(bFlipX)
{
	this->fVecTranslate = fVecTranslate;
	this->fVecScale = fVecScale;
	this->fRot = fRot;
}

Prop::~Prop()
{
}

void Prop::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strImageName, this->fVecTranslate.x, this->fVecTranslate.y);
	pSpriteRenderer->setAngle(this->fRot);
	if (bFlipX)
	{
		pSpriteRenderer->setFlipX(true);
	}
	RenderSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
	this->attachComponent((AComponent*)pSpriteRenderer);
}

