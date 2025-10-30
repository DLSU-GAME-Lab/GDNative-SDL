#pragma once
#include "AGameObject.h"
#include "DialogueRenderer.h"
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
	void setColor(SDL_Color color);
	void initialize() override;
	void modifyText(std::string strNewText);
};

