#pragma once
#include "AGameObject.h"
class StoreItem :public AGameObject
{
private:
	int nPrice;
	Vector2D vecPos;
public:
	StoreItem(std::string strName, int nPrice, Vector2D vecPos);
	~StoreItem();

	// Inherited via AGameObject
	void initialize() override;
};

