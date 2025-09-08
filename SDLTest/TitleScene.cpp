#include "TitleScene.h"
#include "Background.h"
#include "UIButton.h"
#include "TextureManager.h"
#include "GameObjectManager.h"

TitleScene::TitleScene() : AScene(SceneTag::TITLE_SCENE) {}

TitleScene::~TitleScene() {}

void TitleScene::onLoadResources()
{
    TextureManager::getInstance()->load("title_background.png");
    TextureManager::getInstance()->load("start_button.png");
    TextureManager::getInstance()->load("logo.png");
}

void TitleScene::onLoadObjects()
{
    Background* pBackground = new Background("Title Background", "title_background.png");
    UIButton* pStartButton = new UIButton("Start Button", 800, 600, 200, 80, false);

    GameObjectManager::getInstance()->addObject(pBackground);
    GameObjectManager::getInstance()->addObject(pStartButton);
}

void TitleScene::onUnloadResources()
{
    TextureManager::getInstance()->unload("title_background.png");
    TextureManager::getInstance()->unload("start_button.png");
    TextureManager::getInstance()->unload("logo.png");
}

void TitleScene::update(float deltaTime)
{
    AScene::update(deltaTime); // call base update

    // title-screen specific logic here
}