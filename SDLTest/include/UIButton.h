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
	int nX, nY, nH, nW;
	bool bFlipX;
	SceneTag targetScene;

public:
	UIButton(const std::string& strName, int nX, int nY, int nW, int nH, bool bFlipX, SceneTag targetScene = SceneTag::LOBBY_SCENE);
	~UIButton();

	void initialize() override;
	void processInput(SDL_Event eEvent) override;
};

