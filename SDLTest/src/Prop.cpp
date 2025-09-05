#include "Prop.h"


Prop::Prop(const std::string& strName, const std::string& strImageName, int nX, int nY, int nW, int nH, bool bFlipX):
	AGameObject(strName), strImageName(strImageName),nX(nX), nY(nY), nW(nW), nH(nH),bFlipX(bFlipX)
{
}

Prop::~Prop()
{
}

void Prop::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strImageName, nX, nY, nW, nH);
	if (bFlipX)
	{
		pSpriteRenderer->setFlipX(true);
	}
	SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
	this->attachComponent((AComponent*)pSpriteRenderer);
}
