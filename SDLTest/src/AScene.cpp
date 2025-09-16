#include "AScene.h"
#include "SpriteRendererSystem.h"

AScene::AScene(SceneTag ETag) : ETag(ETag) {}

void AScene::onUnloadObjects() {
    GameObjectManager::getInstance()->deleteAllObjects();
}

void AScene::registerObject(AGameObject* pGameObject) {
    GameObjectManager::getInstance()->addObject(pGameObject);
}

SceneTag AScene::getTag() const {
    return this->ETag;
}