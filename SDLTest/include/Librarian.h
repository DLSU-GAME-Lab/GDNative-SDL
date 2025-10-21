#pragma once
#include "AGameObject.h"
#include "Vector2D.h"
class Librarian : public AGameObject
{
public:
	Librarian(Vector2D fVecTranslate, Vector2D fVecScale, float fRot);

	void initialize() override;
};

