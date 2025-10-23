#include "Text.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "Settings.h"

Text::Text(const std::string& strName, const std::string& strMessage, Vector2D fVecTranslate, Vector2D fVecScale, float fRot, bool bFlipX):
AGameObject(strName)
{
	this->strMessage = strMessage;
	this->fVecTranslate = fVecTranslate;
	this->fVecScale = fVecScale;
	this->color = colorBlack;
}

Text::~Text()
{
	TextureManager::getInstance()->unload(this->strName + " Text");
	AGameObject::~AGameObject();
}


void Text::setFont(std::string strFontName)
{
	this->strFontName = strFontName;
}

void Text::setColor(SDL_Color color)
{
	this->color = color;
}

void Text::initialize()
{
	TextureManager::getInstance()->loadFromText(this->strName + " Text", this->strFontName, this->strMessage, this->color);
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strName + " Text");
	this->attachComponent((AComponent*)pSpriteRenderer);
}

void Text::modifyText(std::string strNewText)
{
	TextureManager::getInstance()->unload(this->strName + " Text");
	TextureManager::getInstance()->loadFromText(this->strName + " Text", this->strFontName, strNewText, this->color);
	SpriteRenderer* pRenderer = (SpriteRenderer*)this->findComponentByName("SpriteRenderer");
	SDL_Texture* pTex = TextureManager::getInstance()->get(this->strName + " Text");
	pRenderer->setTexture(pTex);

}
