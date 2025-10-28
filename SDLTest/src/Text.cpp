#include "Text.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "Settings.h"

Text::Text(const std::string& strName, const std::string& strMessage, std::string strFontName, int nFontSize, float fRot, bool bFlipX):
AGameObject(strName)
{
	this->strMessage = strMessage;
	this->fVecScale = Vector2D(1);
	this->color = colorBlack;
	this->nFontSize = nFontSize;
	this->strFontName = strFontName;
}

Text::~Text()
{
	TextureManager::getInstance()->unload(this->strName + " Text");
	AGameObject::~AGameObject();
}

void Text::setColor(SDL_Color color)
{
	this->color = color;
}

void Text::initialize()
{
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

	FontManager::getInstance()->loadFont(this->strFontName, key,nFontSize);
	DialogueRenderer* pDialogueRenderer = new DialogueRenderer();
	pDialogueRenderer->loadFromText(this->strName, key, this->nFontSize, this->strMessage, this->color);
	this->attachComponent((AComponent*)pDialogueRenderer);
}

void Text::modifyText(std::string strNewText)
{
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
	DialogueRenderer* pRenderer = (DialogueRenderer*)this->findComponentByName("DialogueRenderer");
	pRenderer->resetText();
	pRenderer->loadFromText(this->strName, key, this->nFontSize, strNewText, this->color);
}
