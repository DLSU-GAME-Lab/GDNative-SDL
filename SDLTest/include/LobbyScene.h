#pragma once
#include "AScene.h"
#include "EnumSceneTag.h"

class LobbyScene : public AScene
{
public:
    LobbyScene();
    ~LobbyScene();

// Helper Scenes
public:
    void loadReturnDialogue();
    void createButtons();
    void createScene();
    void createExitMenu();
public:
    virtual void onLoadResources() override;
    virtual void onLoadObjects() override;
    virtual void onUnloadResources() override;

};

