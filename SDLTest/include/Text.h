#pragma once
#include "AGameObject.h"

class Text:public AGameObject
{
private:
	std::string strFontName;
	std::string strMessage;
	SDL_Color color;
public:
	Text(const std::string& strName, const std::string& strMessage, Vector2D fVecTranslate, Vector2D fVecScale = Vector2D(1.0f), float fRot = 0.0f, bool bFlipX = false);
	~Text() override;

	void setFont(std::string strFontName);
	void setColor(SDL_Color color);
	void initialize() override;
};

