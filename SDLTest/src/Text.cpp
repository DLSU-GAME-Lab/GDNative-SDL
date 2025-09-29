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
}

Text::~Text()
{
	std::cout << "Destroying: "<<this->strName << std::endl;
	TextureManager::getInstance()->unload(this->strName + " Text");
}


void Text::setFont(std::string strFontName)
{
	this->strFontName = strFontName;
}

void Text::initialize()
{
	TextureManager::getInstance()->loadFromText(this->strName + " Text", this->strFontName, this->strMessage, colorBlack);
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strName + " Text", this->fVecTranslate.x, this->fVecTranslate.y);
	this->attachComponent((AComponent*)pSpriteRenderer);
}
