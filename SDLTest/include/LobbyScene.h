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
    void createDiary();
    void createPageOne(AGameObject* pParent);
    void createPageTwo(AGameObject* pParent);
    void createPageThree(AGameObject* pParent);
    void createPageFour(AGameObject* pParent);
    void createPageFive(AGameObject* pParent);
    void createPageSix(AGameObject* pParent);
    void createPageSeven(AGameObject* pParent);

};

