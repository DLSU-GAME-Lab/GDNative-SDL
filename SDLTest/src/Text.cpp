#include "Text.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "Settings.h"

Text::Text(const std::string& strName, std::string strFontName, int nFontSize, float fRot, bool bFlipX) :
	AGameObject(strName)
{
	this->fVecScale = Vector2D(1);
	this->color = colorBlack;
	this->nFontSize = nFontSize;
	this->strFontName = strFontName;
	this->strFontKey = FontManager::getFontKey(strFontName);
	this->fVecPivot = Vector2D(0.5f, 0.5f);
	this->fRot = fRot;
}

Text::~Text()
{
	//Base class destructor is called automatically, don't call it explicitly
}

void Text::setMessage(const std::string& strMessage)
{
	this->strMessage = strMessage;
	this->refreshText();
}

// same as setMessage, kept for existing callers
void Text::modifyText(std::string strNewText)
{
	this->setMessage(strNewText);
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
	this->fVecPivot = fVecPivot;
	if (TextRenderer* pRenderer = (TextRenderer*)this->findComponentByName("TextRenderer"))
	{
		pRenderer->setPivot(fVecPivot);
	}
}

void Text::refreshText()
{
	TextRenderer* pRenderer = (TextRenderer*)this->findComponentByName("TextRenderer");
	if (!pRenderer) return;

	FontManager::getInstance()->loadFont(this->strFontName, this->strFontKey, this->nFontSize);
	pRenderer->loadFromText(this->strFontKey, this->nFontSize, this->strMessage, this->color);
}

void Text::initialize()
{
	if (this->findComponentByName("TextRenderer")) return;

	TextRenderer* pTextRenderer = new TextRenderer();
	this->attachComponent((AComponent*)pTextRenderer);

	// apply whatever was set before the text was attached
	pTextRenderer->setColor(this->color);
	pTextRenderer->setPivot(this->fVecPivot);
	if (!this->strMessage.empty())
	{
		this->refreshText();
	}
}