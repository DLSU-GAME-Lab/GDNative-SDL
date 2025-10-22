#pragma once
#include "AGameObject.h"
class AnimatedSprite : public AGameObject
{
private:
	std::string strSpriteName;
	int nFrameRate;

public:
	AnimatedSprite(
		std::string strName,
		std::string strSpriteName,
		Vector2D fVecTranslate = Vector2D(0.0f),
		Vector2D fVecScale = Vector2D(1.0f),
		float fRot = 0.0f,
		int nFrameRate = 8);

	void initialize() override;
};

