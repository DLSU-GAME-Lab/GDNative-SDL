#pragma once
#include "AGameObject.h"
class PuzzleToken : public AGameObject
{
public:
	PuzzleToken(std::string strName, Vector2D position, Vector2D scale);

	void initialize() override;
};

