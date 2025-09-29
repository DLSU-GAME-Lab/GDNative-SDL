#pragma once
#pragma once
#include "AScene.h"
#include "EnumSceneTag.h"
class LeftRoomScene :public AScene
{
public: 
	LeftRoomScene();
	~LeftRoomScene();
public:
	virtual void onLoadResources() override;
	virtual void onLoadObjects() override;
	virtual void onUnloadResources() override;
public:
	void loadFonts();
	void loadAnimatedTextures();
	void loadSceneTextures();
	void createButtons();
	void createScene();
	void createExitMenu();
};

