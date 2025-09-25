#include "Text.h"

Text::Text(const std::string& strName, const std::string& strImageName, Vector2D fVecTranslate, Vector2D fVecScale, float fRot, bool bFlipX):
AGameObject(strName)
{
	this->strImageName = strImageName;
	this->fVecTranslate = fVecTranslate;
	this->fVecScale = fVecScale;
}

Text::~Text()
{
}

void Text::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strImageName, this->fVecTranslate.x, this->fVecTranslate.y);
	this->attachComponent((AComponent*)pSpriteRenderer);
}
