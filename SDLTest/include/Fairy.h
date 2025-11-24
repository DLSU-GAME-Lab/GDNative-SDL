#pragma once
#include "AGameObject.h"
class Fairy : public AGameObject
{
public:
	Fairy(Vector2D fVecTranslate, Vector2D fVecScale, float fRot);

	void initialize() override;
};

