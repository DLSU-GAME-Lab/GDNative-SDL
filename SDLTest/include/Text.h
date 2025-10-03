#pragma once
#include "AGameObject.h"
#include "RenderSystem.h"
#include "string.h"

class Text:public AGameObject
{
private:
	std::string strFontName;
	std::string strMessage;
	SDL_Color color;
public:
	Text(const std::string& strName, const std::string& strMessage, Vector2D fVecTranslate, Vector2D fVecScale, float fRot, bool bFlipX);
	~Text();

	void setFont(std::string strFontName);
	void setColor(SDL_Color color);
	void initialize() override;
};

