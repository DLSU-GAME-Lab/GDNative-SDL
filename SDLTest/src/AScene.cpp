#include "AScene.h"

AScene::AScene(SceneTag ETag) {
    this->ETag = ETag;
}

AScene::~AScene() {}

void AScene::onUnloadObjects() {
    GameObjectManager::getInstance()->deleteAllObjects();
}

void AScene::registerObject(AGameObject* pGameObject) {
    GameObjectManager::getInstance()->addObject(pGameObject);
}

SceneTag AScene::getTag() {
    return this->ETag;
}