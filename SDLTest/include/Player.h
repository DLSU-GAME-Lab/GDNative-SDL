#pragma once
#include "AGameObject.h"

class Player : public AGameObject
{
public:
	Player(Vector2D fVecTranslate, Vector2D fVecScale, float fRot);
	~Player();

	void initialize() override;
};

