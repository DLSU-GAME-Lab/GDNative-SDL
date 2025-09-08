#pragma once

#include "AGameObject.h"
#include "SpriteRendererSystem.h"
#include "Settings.h"
#include "SceneManager.h"
#include "EnumSceneTag.h"

class UIButton: public AGameObject
{
private:
	std::string strImageName;
	float fX, fY, fH, fW;
	bool bFlipX;
	SceneTag targetScene;

public:
	UIButton(const std::string& strName, float nX, float nY, float nW, float nH, float fRot, bool bFlipX, SceneTag targetScene = SceneTag::LOBBY_SCENE);
	~UIButton();

	void initialize() override;
	void processInput(SDL_Event eEvent) override;
};

