#pragma once
#include "AScene.h"
#include "EnumSceneTag.h"
class RightRoomScene : public AScene
{
public:
	RightRoomScene();
	~RightRoomScene();
public:
	virtual void onLoadResources() override;
	virtual void onLoadObjects() override;
	virtual void onUnloadResources() override;
public:
	void loadReturnDialogue();
	void loadAnimatedTextures();
	void loadSceneTextures();
	void createButtons();
	void createScene();
	void createExitMenu();
};


