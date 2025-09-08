#pragma once
#include "AGameObject.h"

class Player : public AGameObject
{
public:
	Player(float fPosX, float fPosY, float fScaleX, float fScaleY, float fRot);
	~Player();

	void initialize() override;
};

