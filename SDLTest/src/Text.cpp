#include "Text.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "Settings.h"

Text::Text(const std::string& strName, std::string strFontName, int nFontSize, float fRot, bool bFlipX):
AGameObject(strName)
{
	this->fVecScale = Vector2D(1);
	this->color = colorBlack;
	this->nFontSize = nFontSize;
	this->strFontName = strFontName;
	this->fRot = fRot;
}

Text::~Text()
{
	AGameObject::~AGameObject();
}

void Text::setMessage(const std::string& strMessage)
{
	this->strMessage = strMessage;
	std::string key = this->strFontName;

	size_t ttfPos = key.rfind(".ttf");
	size_t otfPos = key.rfind(".otf");

	if (ttfPos != std::string::npos && ttfPos == key.length() - 4)
	{
		key = key.substr(0, ttfPos);
	}
	else if (otfPos != std::string::npos && otfPos == key.length() - 4)
	{
		key = key.substr(0, otfPos);
	}

	FontManager::getInstance()->loadFont(this->strFontName, key, nFontSize);
	TextRenderer* pRenderer = (TextRenderer*)this->findComponentByName("TextRenderer");
	pRenderer->resetText();
	pRenderer->loadFromText(key, this->nFontSize, this->strMessage, this->color);
}

std::string Text::getMessage()
{
	return this->strMessage;
}

void Text::setColor(SDL_Color color)
{
	this->color = color;
	if (TextRenderer* pRenderer = (TextRenderer*)this->findComponentByName("TextRenderer"))
	{
		pRenderer->setColor(color);
	}
}

void Text::setPivot(Vector2D fVecPivot)
{
	TextRenderer* pRenderer = (TextRenderer*)this->findComponentByName("TextRenderer");
	if (pRenderer)
	{
		pRenderer->setPivot(fVecPivot);
	}
}

void Text::initialize()
{
	TextRenderer* pTextRenderer = new TextRenderer();
	this->attachComponent((AComponent*)pTextRenderer);
}
