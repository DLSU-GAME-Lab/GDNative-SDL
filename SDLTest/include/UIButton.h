#pragma once
#include "AGameObject.h"
#include "SpriteRendererSystem.h"
#include "Settings.h"
class UIButton: public AGameObject
{
private:
	std::string strImageName;
	int nX, nY, nH, nW;
public:
	UIButton(const std::string& strName, int nX, int nY, int nW, int nH);
	~UIButton();

	void initialize() override;
};

