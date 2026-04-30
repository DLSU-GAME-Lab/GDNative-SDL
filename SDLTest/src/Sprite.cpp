#include "Sprite.h"
#include "SpriteRenderer.h"
#include "TextureManager.h"

Sprite::Sprite(const std::string& strName, const std::string& strImageName, Vector2D fVecTranslate, Vector2D fVecScale, float fRot, bool bFlipX):
	AGameObject(strName), strImageName(strImageName),bFlipX(bFlipX)
{
	this->fVecTranslate = fVecTranslate;
	this->fVecScale = fVecScale;
	this->fRot = fRot;
}

void Sprite::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strImageName);
	this->attachComponent((AComponent*)pSpriteRenderer);

	if (bFlipX)
	{
		pSpriteRenderer->setFlipX(true);
	}

	
}

void Sprite::setColor(SDL_Color color)
{
	SpriteRenderer* pSpriteRenderer = (SpriteRenderer*)this->findComponentByName("SpriteRenderer");
	if (pSpriteRenderer) pSpriteRenderer->setColor(color);
}

void Sprite::setImage(std::string strImageName)
{
	this->strImageName = strImageName;
	SpriteRenderer* pSpriteRenderer = (SpriteRenderer*)this->findComponentByName("SpriteRenderer");
	SDL_Texture* pTexture = TextureManager::getInstance()->get(strImageName);
	if (pSpriteRenderer) pSpriteRenderer->setTexture(pTexture);
}
