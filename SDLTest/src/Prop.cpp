#include "Prop.h"
#include "Collider.h"


Prop::Prop(const std::string& strName, const std::string& strImageName, Vector2D fVecTranslate, Vector2D fVecScale, float fRot, bool bFlipX):
	AGameObject(strName), strImageName(strImageName),bFlipX(bFlipX)
{
	this->fVecTranslate = fVecTranslate;
	this->fVecScale = fVecScale;
	this->fRot = fRot;
}

void Prop::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strImageName);
	this->attachComponent((AComponent*)pSpriteRenderer);

	if (bFlipX)
	{
		pSpriteRenderer->setFlipX(true);
	}

	
}




