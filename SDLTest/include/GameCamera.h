#pragma once
#include "AGameObject.h"
class GameCamera : public AGameObject
{
public:
	GameCamera();
	~GameCamera();

	void initialize() override;
};

