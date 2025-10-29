#include "AScene.h"

AScene::AScene(SceneTag ETag) : ETag(ETag) {}

void AScene::onUnloadObjects() 
{
    EventBroadcaster::getInstance()->unregisterAllListeners();
    GameObjectManager::getInstance()->deleteAllObjects();
}

void AScene::registerObject(AGameObject* pGameObject) {
    GameObjectManager::getInstance()->addObject(pGameObject);
}

SceneTag AScene::getTag() const {
    return this->ETag;
}