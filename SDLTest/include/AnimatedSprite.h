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
		Vector2D fVecTranslate,
		Vector2D fVecScale,
		float fRot,
		int nFrameRate);
	~AnimatedSprite();

	void initialize() override;
};

