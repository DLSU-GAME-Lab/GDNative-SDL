#pragma once
#include "AGameObject.h"
#include "TextRenderer.h"

class Text :public AGameObject
{
private:
	std::string strFontName;
	std::string strFontKey;
	std::string strMessage;
	int nFontSize;
	SDL_Color color;
	Vector2D fVecPivot;

	// re-renders the stored message, does nothing until the text renderer exists
	void refreshText();
public:
	Text(const std::string& strName, std::string strFontName, int nFontSize, float fRot = 0.0f, bool bFlipX = false);
	virtual ~Text();

	// message, color and pivot can be set before the text is attached or initialized
	void setMessage(const std::string& strMessage);
	void modifyText(std::string strNewText);
	std::string getMessage();
	void setColor(SDL_Color color);
	void setPivot(Vector2D fVecPivot);
	void initialize() override;
};