#pragma once
#include "AGameObject.h"
#include "TextRenderer.h"
class Text:public AGameObject
{
private:
	std::string strFontName;
	std::string strMessage;
	int nFontSize;
	SDL_Color color;
public:
	Text(const std::string& strName, std::string strFontName, int nFontSize, float fRot = 0.0f, bool bFlipX = false);
	~Text() override;

	void setMessage(const std::string& strMessage);
	std::string getMessage();
	void setColor(SDL_Color color);
	void setPivot(Vector2D fVecPivot);
	void initialize() override;
};

