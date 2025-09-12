#pragma once
#include "AGameObject.h"
class Librarian : public AGameObject
{
public:
	Librarian(float fPosX, float fPosY, float fScaleX, float fScaleY, float fRot);
	~Librarian();

	void initialize() override;
};

