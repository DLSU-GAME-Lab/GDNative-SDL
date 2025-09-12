#pragma once
#include "AGameObject.h"
class Fairy : public AGameObject
{
public:
	Fairy(float fPosX, float fPosY, float fScaleX, float fScaleY, float fRot);
	~Fairy();

	void initialize() override;
};

