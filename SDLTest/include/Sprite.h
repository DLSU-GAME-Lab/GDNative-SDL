#pragma once
#include "AGameObject.h"

class Sprite : public AGameObject
{
private:
	std::string strImageName;
	bool bFlipX;
	int dCount = 0;
public:
	Sprite(const std::string& strName, const std::string& strImageName, Vector2D fVecTranslate = { 0, 0 }, Vector2D fVecScale = { 1, 1 }, float fRot = 0.0f, bool bFlipX = false);

	void initialize() override;
	void setColor(SDL_Color color);
	void setImage(std::string strImageName);
};
