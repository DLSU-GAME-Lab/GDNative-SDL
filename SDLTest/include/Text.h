#pragma once
#include "AGameObject.h"
#include "RenderSystem.h"
#include "string.h"
class Text:public AGameObject
{
private:
	std::string strImageName;
	
public:
	Text(const std::string& strName, const std::string& strFontName, Vector2D fVecTranslate, Vector2D fVecScale, float fRot, bool bFlipX);
	~Text();
	void setMessage(std::string strMessage);
	void initialize() override;
};

